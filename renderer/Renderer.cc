/*
 *	Author: Dino Wernli
 */

#include "Renderer.h"

#include "proto/Configuration.pb.h"
#include "scene/Scene.h"

// static
Renderer* Renderer::FromConfig(const raytracer::Configuration& config) {
  // TODO(dinow): Parse config and pass corresponding objects to new renderer.
  return new Renderer();
}

Renderer::Renderer() {
}

Renderer::~Renderer() {
}
