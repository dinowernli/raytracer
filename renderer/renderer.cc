/*
 * Author: Dino Wernli
 */

#include "renderer.h"

#include "proto/configuration.pb.h"
#include "renderer/sampler/sampler.h"
#include "renderer/sampler/scanline_sampler.h"
#include "renderer/updatable.h"
#include "scene/scene.h"

Renderer::Renderer(Scene* scene, Sampler* sampler)
    : scene_(scene), sampler_(sampler) {
  // TODO(dinow): Move this call to the start of the rendering process.
  const Camera* camera = &scene_->camera();
  sampler_->Init(camera);
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

// static
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  // TODO(dinow): Parse config and pass corresponding objects to new renderer.
  Renderer* result = new Renderer(new Scene(), new ScanlineSampler());
  return result;
}
