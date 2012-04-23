#include <memory>

#include "proto/Configuration.pb.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"

int main(int argc, char **argv) {
  raytracer::Configuration config;
  Scene scene;
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  return 0;
}
