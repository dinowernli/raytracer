/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <algorithm>
#include <glog/logging.h>
#include <memory>
#include <thread>
#include <unistd.h>

#include "proto/config/renderer_config.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/shader/phong_shader.h"
#include "renderer/shader/shader.h"
#include "renderer/updatable.h"
#include "scene/scene.h"
#include "util/ray.h"

Renderer::Renderer(Sampler* sampler, Shader* shader, size_t num_threads,
                   size_t recursion_depth)
    : sampler_(sampler), shader_(shader), num_threads_(num_threads),
      recursion_depth_(recursion_depth) {
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

  UpdateListeners();

  std::vector<std::thread> workers;
  for (size_t i = 0; i < num_threads_; ++i) {
    workers.push_back(std::thread(&Renderer::WorkerMain, this, i));
  }

  LOG(INFO) << "Creating " << num_threads_ << " workers";

  while (!sampler_->IsDone()) {
    UpdateListeners();
    usleep(kMicroToMilli * kSleepTimeMilli);
  }

  for (auto it = workers.begin(); it != workers.end(); ++it) {
    it->join();
  }

  LOG(INFO) << "All workers terminated";

  UpdateListeners();

  scene_ = NULL;
  LOG(INFO) << "Ending rendering process";
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
      Sample& sample = samples[i];
      Ray ray = camera->GenerateRay(sample);
      refraction_stack.clear();
      refraction_stack.push_back(scene_->refraction_index());
      sample.set_color(TraceColor(ray, 0, &refraction_stack));
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

void Renderer::UpdateListeners() const {
  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Update(*sampler_);
  }
}

// static
const size_t Renderer::kSleepTimeMilli = 300;

// static
const size_t Renderer::kMicroToMilli = 1000;

// static
Renderer* Renderer::FromConfig(const raytracer::RendererConfig& config) {
  Sampler* sampler = new ScanlineSampler(config.threads() > 1);
  Shader* shader = new PhongShader(config.shadows());
  return new Renderer(sampler, shader, config.threads(),
                      config.recursion_depth());
}
