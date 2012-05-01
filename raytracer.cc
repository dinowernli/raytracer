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
  // LOG(INFO): Always logged.
  // DVLOG(i): Only compiled in if DEBUG flag set, logged if log level is >= i.
  //
  // Always run "GLOG_logtostderr=1 ./build/raytracer" (assumed below).
  // Run "GLOG_v=i ./build/raytracer" for verbose loggin up to level i.
  google::InitGoogleLogging(argv[0]);

  raytracer::Configuration config;
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  renderer->AddListener(new PpmExporter("output/test.ppm"));
  renderer->AddListener(new BmpExporter());
  renderer->AddListener(new ProgressListener());
  renderer->Render();

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
