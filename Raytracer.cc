/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <memory>

#include "proto/Configuration.pb.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"
#include "util/Vector3.h"

int main(int argc, char **argv) {
  raytracer::Configuration config;
  Scene scene;
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  Vector3 vec(1, 2, 4);
  return 0;
}
