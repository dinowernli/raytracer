/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/stubs/common.h>
#include <memory>

#include "listener/bmp_exporter.h"
#include "listener/ppm_exporter.h"
#include "listener/progress_listener.h"
#include "proto/configuration.pb.h"
#include "proto/scene/triangle_data.pb.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

using raytracer::RendererConfig;
using raytracer::SceneConfig;
using raytracer::TriangleData;

DEFINE_bool(shadows, true, "Whether or not shadows are rendered");

DEFINE_bool(use_kd_tree, true, "Whether or not to use a KdTree in the scene");

DEFINE_uint64(worker_threads, 8, "Number of rendering worker threads to use");

DEFINE_string(bmp_file, "image", "If <file> is passed, a BMP image will be "
                                  "saved at 'output/<file>.bmp'");

DEFINE_string(ppm_file, "", "If <file> is passed, a PPM image will be saved at "
                            "'output/<file>.ppm'");

int main(int argc, char **argv) {
  // LOG(INFO): Always logged.
  // DVLOG(i): Only compiled in if DEBUG flag set, logged if log level is >= i.
  //
  // Always run "GLOG_logtostderr=1 ./build/raytracer" (assumed below).
  // Run "GLOG_v=i ./build/raytracer" for verbose logging up to level i.
  google::InitGoogleLogging(argv[0]);

  // Initializes the flags libraray. The last argument tells the library to
  // remove the flags from argc and argv, so that only real arguments remain.
  google::ParseCommandLineFlags(&argc, &argv, true);

  SceneConfig s_config;
  if (FLAGS_use_kd_tree) {
    s_config.mutable_kd_tree_config();
  }

  // Just for debugging purposes.
  TriangleData data;

  //std::unique_ptr<Scene> scene(Scene::QuadricsScene(s_config));
  std::unique_ptr<Scene> scene(Scene::HorseScene(s_config));
  //std::unique_ptr<Scene> scene(Scene::TestScene(s_config));

  RendererConfig r_config;
  r_config.set_threads(FLAGS_worker_threads);
  r_config.set_shadows(FLAGS_shadows);

  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(r_config));
  if (!FLAGS_bmp_file.empty()) {
    renderer->AddListener(new BmpExporter("output/" + FLAGS_bmp_file + ".bmp"));
  }

  if (!FLAGS_ppm_file.empty()) {
    renderer->AddListener(new PpmExporter("output/" + FLAGS_ppm_file + ".ppm"));
  }

  renderer->AddListener(new ProgressListener());
  renderer->Render(scene.get());

  // Free all memory in the protocol buffer library.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
