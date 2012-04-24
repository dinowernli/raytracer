/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <memory>

#include "exporter/BmpExporter.h"
#include "proto/Configuration.pb.h"
#include "renderer/Image.h"
#include "renderer/Renderer.h"
#include "scene/Camera.h"
#include "scene/PointLight.h"
#include "scene/Scene.h"
#include "util/Point3.h"
#include "util/Ray.h"
#include "util/Vector3.h"

int main(int argc, char **argv) {
  raytracer::Configuration config;
  Scene scene;

  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));

  Image image(300, 400);

  Vector3 vec(1, 2, 4);
  Point3 p(0, 0, 0);

  Camera* cam = new Camera(p, vec, vec, 0, 300, 500);
  Light* light = new PointLight(Point3(0, 0, 0), Color3(0, 0, 0));

  scene.AddLight(light);
  scene.set_camera(cam);

  scene.Init();

  BmpExporter* exporter = new BmpExporter();
  renderer->AddListener(exporter);

  return 0;
}
