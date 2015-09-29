// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <algorithm>
#include <glog/logging.h>
#include <memory>
#include <thread>
#include <unistd.h>

#include "listener/bmp_exporter.h"
#include "proto/config/renderer_config.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/sampler/progressive_sampler.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/sampler/supersampler.h"
#include "renderer/shader/phong_shader.h"
#include "renderer/shader/shader.h"
#include "renderer/statistics.h"
#include "renderer/updatable.h"
#include "scene/light/light.h"
#include "scene/material.h"
#include "scene/scene.h"
#include "util/ray.h"

Renderer::Renderer(Sampler* sampler, Supersampler* supersampler,
                   Shader* shader, size_t num_threads, size_t recursion_depth,
                   Statistics* stats)
    : sampler_(sampler), supersampler_(supersampler), shader_(shader),
      num_threads_(num_threads), recursion_depth_(recursion_depth),
      statistics_(stats) {
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
  scene_->Init();

  const Camera* camera = &scene_->camera();
  if (camera == NULL) {
    sampler_->Init(0, 0);
  } else {
    sampler_->Init(camera->resolution_x(), camera->resolution_y());
  }

  if (HasStatistics()) {
    statistics_->Init(camera->resolution_x(), camera->resolution_y());
  }

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

  if (HasStatistics()) {
    LOG(INFO) << "Exporting statistics";
    statistics_->Export();
  }

  scene_ = NULL;
  LOG(INFO) << "Finished rendering";
}

void Renderer::WorkerMain(size_t worker_id) {
  DVLOG(1) << "Worker " << worker_id << " allocating buffer of size "
           << sampler_->MaxJobSize();
  // Buffers a set of samples which represent image pixels.
  std::vector<Sample> samples(sampler_->MaxJobSize());

  // Buffers the samples (for a single pixel) produced by the supersampler.
  std::vector<Sample> subsamples;

  // Fetch the camera for easier access.
  const Camera* camera = &scene_->camera();

  // Use a single refraction stack since calling "clear" only affects size, not
  // the capacity. Therefore, the number of allocations will not grow (a lot)
  // once the first ray is fully traced.
  std::vector<Scalar> refraction_stack;

  size_t n_samples = 0;
  while((n_samples = sampler_->NextJob(&samples)) > 0) {
    for (size_t i = 0; i < n_samples; ++i) {
      Sample& main_sample = samples[i];
      Supersampler supersampler(*supersampler_);
      DVLOG(3) << "Processing sample " << main_sample;

      size_t current_subsamples = 0;
      while((current_subsamples = supersampler.GenerateSubsamples(
                                              main_sample, &subsamples)) > 0) {
        for (size_t j = 0; j < current_subsamples; ++j) {
          DVLOG(3) << "Processing subsample " << subsamples[j];
          Ray ray = camera->GenerateRay(subsamples[j]);
          refraction_stack.clear();
          refraction_stack.push_back(scene_->refraction_index());
          subsamples[j].set_color(TraceColor(ray, 0, &refraction_stack));
        }
        supersampler.ReportResults(subsamples, current_subsamples);
      }
      main_sample.set_color(supersampler.MeanResults());
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
  if(data.IntersectedLight()) {
    // Direct hit of some light source.
    return data.light()->color();
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
  Statistics* stats = NULL;
  if (config.has_sampling_heatmap_path()) {
    stats = new Statistics(new BmpExporter(config.sampling_heatmap_path()));
  }

  Sampler* sampler = NULL;
  if (config.sampler_type() == raytracer::RendererConfig::SCANLINE) {
    sampler = new ScanlineSampler(config.threads() > 1);
  } else if (config.sampler_type() == raytracer::RendererConfig::PROGRESSIVE) {
    sampler = new ProgressiveSampler(config.threads() > 1);
  }
  CHECK(sampler != NULL) << "Could not load sampler";

  Shader* shader = new PhongShader(config.shadows());
  Supersampler* supersampler = new Supersampler(
      config.root_rays_per_pixel(),
      config.adaptive_supersampling_threshold(),
      stats);

  return new Renderer(sampler, supersampler, shader, config.threads(),
                      config.recursion_depth(), stats);
}
