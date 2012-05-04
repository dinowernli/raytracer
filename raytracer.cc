/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <glog/logging.h>
#include <google/protobuf/stubs/common.h>
#include <memory>

#include "listener/ppm_exporter.h"
#include "listener/progress_listener.h"
#include "proto/configuration.pb.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

using raytracer::RendererConfig;
using raytracer::SceneConfig;

int main(int argc, char **argv) {
  // LOG(INFO): Always logged.
  // DVLOG(i): Only compiled in if DEBUG flag set, logged if log level is >= i.
  //
  // Always run "GLOG_logtostderr=1 ./build/raytracer" (assumed below).
  // Run "GLOG_v=i ./build/raytracer" for verbose logging up to level i.
  google::InitGoogleLogging(argv[0]);

  SceneConfig s_config;
  s_config.mutable_kd_tree_config();  // Initialize empty config.

  std::unique_ptr<Scene> scene(Scene::QuadricsScene(s_config));
  //std::unique_ptr<Scene> scene(Scene::HorseScene(s_config));
  //std::unique_ptr<Scene> scene(Scene::TestScene(s_config));

  RendererConfig r_config;
  r_config.set_threads(4);
  r_config.set_shadows(true);

  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(r_config));
  renderer->AddListener(new PpmExporter("output/test.ppm"));
  renderer->AddListener(new ProgressListener());
  renderer->Render(scene.get());

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
