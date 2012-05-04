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
#include "scene/scene.h"

int main(int argc, char **argv) {
  // LOG(INFO): Always logged.
  // DVLOG(i): Only compiled in if DEBUG flag set, logged if log level is >= i.
  //
  // Always run "GLOG_logtostderr=1 ./build/raytracer" (assumed below).
  // Run "GLOG_v=i ./build/raytracer" for verbose logging up to level i.
  google::InitGoogleLogging(argv[0]);

  std::unique_ptr<Scene> scene(Scene::QuadricsScene());
  //std::unique_ptr<Scene> scene(Scene::HorseScene());
  //std::unique_ptr<Scene> scene(Scene::TestScene());

  raytracer::Configuration config;
  config.set_threads(4);
  config.set_shadows(true);

  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(config));
  renderer->AddListener(new PpmExporter("output/test.ppm"));
  renderer->AddListener(new BmpExporter());
  renderer->AddListener(new ProgressListener());
  renderer->Render(scene.get());

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
