/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <memory>

#include "exporter/BmpExporter.h"
#include "proto/Configuration.pb.h"
#include "renderer/Image.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"
#include "util/Point3.h"
#include "util/Ray.h"
#include "util/Vector3.h"

int main(int argc, char **argv) {
  raytracer::Configuration config;
  Scene scene;
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  Vector3 vec(1, 2, 4);
  Ray ray(Point3(0, 0, 0), vec);
  Image image(300, 400);

  BmpExporter* exporter = new BmpExporter();
  renderer->AddListener(exporter);

  return 0;
}
