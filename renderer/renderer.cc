/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <glog/logging.h>
#include <memory>
#include <thread>
#include <unistd.h>

#include "proto/configuration.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/shader/phong_shader.h"
#include "renderer/shader/shader.h"
#include "renderer/updatable.h"
#include "scene/scene.h"
#include "util/ray.h"

Renderer::Renderer(Sampler* sampler, Shader* shader, size_t num_threads)
    : sampler_(sampler), shader_(shader), num_threads_(num_threads) {
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
    workers.push_back(std::thread(&Renderer::WorkerMain, this));
  }

  LOG(INFO) << "Created " << workers.size() << " worker threads";

  while (!sampler_->IsDone()) {
    UpdateListeners();
    usleep(kMicroToMilli * kSleepTimeMilli);
  }

  for (auto it = workers.begin(); it != workers.end(); ++it) {
    it->join();
  }

  LOG(INFO) << "All worker threads returned";

  UpdateListeners();

  scene_ = NULL;
  LOG(INFO) << "Ending rendering process";
}

void Renderer::WorkerMain() {
  const Camera* camera = &scene_->camera();

  Sample sample;
  while(sampler_->NextSample(&sample)) {
    Ray ray = camera->GenerateRay(sample);
    sample.set_color(TraceColor(ray));
    sampler_->AcceptSample(sample);
  }
}

Color3 Renderer::TraceColor(const Ray& ray) {
  IntersectionData data(ray);
  scene_->Intersect(ray, &data);
  return shader_->Shade(data, *scene_);
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
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  Sampler* sampler = new ScanlineSampler(config.threads() > 0);
  Shader* shader = new PhongShader();
  return new Renderer(sampler, shader, config.threads());
}
