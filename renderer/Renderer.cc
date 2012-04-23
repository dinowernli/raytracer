/*
 * Author: Dino Wernli
 */

#include "Renderer.h"

#include "proto/Configuration.pb.h"
#include "renderer/Updatable.h"
#include "scene/Scene.h"

Renderer::Renderer() {
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
  return new Renderer();
}
