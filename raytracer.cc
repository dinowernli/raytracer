/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <google/protobuf/stubs/common.h>
#include <memory>

#include "exporter/bmp_exporter.h"
#include "proto/configuration.pb.h"
#include "renderer/image.h"
#include "renderer/renderer.h"
#include "scene/camera.h"
#include "scene/point_light.h"
#include "scene/scene.h"
#include "util/point3.h"
#include "util/ray.h"
#include "util/vector3.h"

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

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
