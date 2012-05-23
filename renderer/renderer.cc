/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <algorithm>
#include <ctime>
#include <glog/logging.h>
#include <memory>
#include <random>
#include <thread>
#include <unistd.h>

#include "proto/config/renderer_config.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/sampler/progressive_sampler.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/shader/phong_shader.h"
#include "renderer/shader/shader.h"
#include "renderer/updatable.h"
#include "scene/scene.h"
#include "util/ray.h"

Renderer::Renderer(Sampler* sampler, Shader* shader, size_t num_threads,
                   size_t recursion_depth, size_t rays_per_pixel)
    : sampler_(sampler), shader_(shader), num_threads_(num_threads),
      recursion_depth_(recursion_depth), rays_per_pixel_(rays_per_pixel) {
  if (num_threads == 0) {
    LOG(WARNING) << "Can't render with 0 workers. Using 1 instead.";
    num_threads_ = 1;
  }
}

Renderer::~Renderer() {
}

void Renderer::AddListener(Updatable* listener) {
  for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    if (listener == it->get()) {
      return;
    }
  }
  listeners_.push_back(std::unique_ptr<Updatable>(listener));
}

void Renderer::Render(Scene* scene) {
  LOG(INFO) << "Starting rendering process";
  scene_ = scene;

  // Perform some sanity checks before starting.
  CHECK(num_threads_ > 0) << "Can't render with 0 workers.";
  if (num_threads_ > 1) {
    CHECK(sampler_->IsThreadSafe()) << "Attempting to render with multiple "
                                    << "threads on unsafe sampler.";
  }

  // We can skip the NULL-check for camera because we may assume that the
  // sampler can handle this. If camera is NULL, the loop below will terminate
  // instantly.
  const Camera* camera = &scene_->camera();
  scene_->Init();
  sampler_->Init(camera);

  // Initialize the pseudorandom generator used for jittering samples.
  std::srand(time(0));
  LOG(INFO) << "Creating " << num_threads_ << " workers";

  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Started(*sampler_);
  }
  std::vector<std::thread> workers;
  for (size_t i = 0; i < num_threads_; ++i) {
    workers.push_back(std::thread(&Renderer::WorkerMain, this, i));
  }

  while (!sampler_->IsDone()) {
    for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
      it->get()->Updated(*sampler_);
    }
    usleep(kSleepTimeMilli * MILLI_TO_MICRO);
  }
  for (auto it = workers.begin(); it != workers.end(); ++it) {
    it->join();
  }
  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Ended(*sampler_);
  }

  LOG(INFO) << "All workers terminated";

  scene_ = NULL;
  LOG(INFO) << "Finished rendering";
}

// Returns a uniformly distributed random sample in [-0.5, 0.5].
static inline Scalar random_jitter() {
  Scalar result = std::rand();
  return (result / RAND_MAX) + 0.5;
}

void Renderer::WorkerMain(size_t worker_id) {
  DVLOG(1) << "Worker " << worker_id << " allocating buffer of size "
           << sampler_->MaxJobSize();
  std::vector<Sample> samples(sampler_->MaxJobSize());
  const Camera* camera = &scene_->camera();

  // Use a single refraction stack since calling "clear" only affects size, not
  // the capacity. Therefore, the number of allocations will not grow (a lot)
  // once the first ray is fully traced.
  std::vector<Scalar> refraction_stack;

  size_t n_samples = 0;
  while((n_samples = sampler_->NextJob(&samples)) > 0) {
    for (size_t i = 0; i < n_samples; ++i) {
      Sample& main_sample = samples[i];
      Color3 accumulated(0, 0, 0);

      // TODO(dinow): Extract inner loop body as own function or as a part of
      // some sort of sampling pipeline.

      // Shoot multiple ray for each pixel for anti-aliasing.
      for (size_t j = 0; j < rays_per_pixel_; ++j) {
        Sample jittered(main_sample);
        if (j > 0) {
          // Make sure one ray actually goes through the middle.
          jittered.set_offset_x(random_jitter());
          jittered.set_offset_y(random_jitter());
        }

        Ray ray = camera->GenerateRay(jittered);
        refraction_stack.clear();
        refraction_stack.push_back(scene_->refraction_index());
        Color3 contribution = TraceColor(ray, 0, &refraction_stack);
        accumulated = accumulated + contribution / rays_per_pixel_;
      }
      main_sample.set_color(accumulated);
    }
    sampler_->AcceptJob(samples, n_samples);
  }
}

Color3 Renderer::TraceColor(const Ray& ray, size_t depth,
                            std::vector<Scalar>* refraction_stack) {
  IntersectionData data(ray);
  if (!scene_->Intersect(ray, &data)) {
    return scene_->background();
  }
  const Material& material = *(data.material);
  using std::max;

  Color3 shaded = shader_->Shade(data, *scene_);
  if (depth >= recursion_depth_) {
    return shaded;
  }

  Color3 refracted;
  Scalar refraction_percentage = max(material.refraction_percentage(), 0.0);
  Color3 reflected;
  Scalar reflection_percentage = max(material.reflection_percentage(), 0.0);

  if (refraction_percentage > 0) {
    Scalar old_index = refraction_stack->back();
    Scalar new_index;
    Scalar entering_product = data.normal.Dot(ray.direction());
    if(entering_product < 0) {
      // Ray enters object.
      new_index = material.refraction_index();
    } else {
      // TODO(dinow): Somehow, this produces an invalid read on horse scene with
      // recursion depth 10. Need to investigate.
      // Update: Cause is lack of distinction between volumetric and non-vol.
      // elements. And the fact that the whole refraction thing is a hack.
      if (refraction_stack->size() >= 2) {
        new_index = (*refraction_stack)[refraction_stack->size() - 2];
      } else {
        DVLOG(2) << "Prevented invalid refraction stack access, size: "
                 << refraction_stack->size() << ", attempted to access index: "
                 << refraction_stack->size() - 2 << " at depth " << depth
                 << " with entering product: " << entering_product;
        new_index = material.refraction_index();
      }
    }

    // TODO(dinow): Check if normalization is really necessary.
    Vector3 normal = data.normal.Normalized();
    Scalar ratio = old_index / new_index;
    Vector3 omega(-1 * ray.direction());
    Scalar dot = omega.Dot(normal);

    if (dot < 0) {
      dot = -dot;
      normal = (-1) * normal;
    }
    Scalar under_root = 1 - (ratio * ratio) * (1 - dot * dot);

    if (under_root < 0) {
      // Total reflection.
      reflection_percentage += refraction_percentage;
      refraction_percentage = 0;
    } else {
      Vector3 dir(((omega - dot * normal) * (-ratio))
                  - sqrt(under_root) * normal);
      Point3 pos(data.position + EPSILON * dir);

      refraction_stack->push_back(new_index);
      refracted = TraceColor(Ray(pos, dir), depth + 1, refraction_stack);
      refraction_stack->pop_back();
    }
  }

  if (reflection_percentage > 0) {
    Vector3 dir = ray.direction().ReflectedOnPlane(data.normal);
    Point3 pos(data.position + EPSILON * dir);
    reflected = TraceColor(Ray(pos, dir), depth + 1, refraction_stack);
  }

  return refraction_percentage * refracted + reflection_percentage * reflected
      + (1 - refraction_percentage - reflection_percentage) * shaded;
}

// static
const size_t Renderer::kSleepTimeMilli = 300;

// static
Renderer* Renderer::FromConfig(const raytracer::RendererConfig& config) {
  Sampler* sampler = NULL;
  if (config.sampler_type() == raytracer::RendererConfig::SCANLINE) {
    sampler = new ScanlineSampler(config.threads() > 1);
  } else if (config.sampler_type() == raytracer::RendererConfig::PROGRESSIVE) {
    sampler = new ProgressiveSampler(config.threads() > 1);
  }
  CHECK(sampler != NULL) << "Could not load sampler";

  Shader* shader = new PhongShader(config.shadows());
  return new Renderer(sampler, shader, config.threads(),
                      config.recursion_depth(), config.rays_per_pixel());
}
