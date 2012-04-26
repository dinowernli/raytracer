/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include <memory>

#include "proto/configuration.pb.h"
#include "renderer/sampler/sample.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/updatable.h"
#include "scene/scene.h"
#include "util/ray.h"

Renderer::Renderer(Scene* scene, Sampler* sampler)
    : scene_(scene), sampler_(sampler) {
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

void Renderer::Start() {
  // We can skip the NULL-check for camera because we may assume that the
  // sampler can handle this. If camera is NULL, the loop below will terminate
  // instantly.
  const Camera* camera = &scene_->camera();
  sampler_->Init(camera);

  for(std::unique_ptr<Sample> sample(sampler_->NextSample());
      sample.get() != NULL;
      sample.reset(sampler_->NextSample())) {
    Ray ray = camera->GenerateRay(*sample);
    sample->set_color(TraceColor(ray));
    sampler_->AcceptSample(*sample);
  }

  for(auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    it->get()->Update(sampler_->image());
  }
}

Color3 Renderer::TraceColor(const Ray& ray) {
  // TODO(dinow): Implement this. Return gray for now.
  return Color3(0.5, 0.5, 0.5);
}

// static
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  // TODO(dinow): Parse config and pass corresponding objects to new renderer.
  Renderer* result = new Renderer(new Scene(), new ScanlineSampler());
  return result;
}
