/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <google/protobuf/stubs/common.h>
#include <memory>

#include "exporter/bmp_exporter.h"
#include "exporter/ppm_exporter.h"
#include "proto/configuration.pb.h"
#include "renderer/intersection_data.h"
#include "renderer/renderer.h"
#include "renderer/shader/phong_shader.h"

// This is just to test some objects.
// TODO(dinow): Remove this when actual functionality... happens.
void TestStuff() {
  PhongShader shader;
  IntersectionData data;
  Color3 color = shader.Shade(data);
}

int main(int argc, char **argv) {
  TestStuff();

  raytracer::Configuration config;
  std::auto_ptr<Renderer> renderer(Renderer::FromConfig(config));
  renderer->AddListener(new PpmExporter("output/test.ppm"));
  renderer->AddListener(new BmpExporter());
  renderer->Start();

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
