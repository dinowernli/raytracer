/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <glog/logging.h>
#include <google/protobuf/stubs/common.h>
#include <memory>

#include "listener/bmp_exporter.h"
#include "listener/ppm_exporter.h"
#include "listener/progress_listener.h"
#include "proto/configuration.pb.h"
#include "renderer/renderer.h"

int main(int argc, char **argv) {
  // The severities are INFO, WARNING, ERROR, FATAL. The compiler is configured
  // to compile out all log statements which which are below WARNING if compiled
  // in release mode, i.e. all INFO log statements are removed.
  google::InitGoogleLogging(argv[0]);

  raytracer::Configuration config;
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  renderer->AddListener(new PpmExporter("output/test.ppm"));
  renderer->AddListener(new BmpExporter());
  renderer->AddListener(new ProgressListener());
  renderer->Start();

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
