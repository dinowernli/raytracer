/*
 * Entry point for the raytracer binary.
 * Autor: Dino Wernli
 */

#include <fstream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/text_format.h>
#include <memory>
#include <streambuf>
#include <string>

#include "listener/bmp_exporter.h"
#include "listener/ppm_exporter.h"
#include "listener/progress_listener.h"
#include "proto/config/renderer_config.pb.h"
#include "proto/config/scene_config.pb.h"
#include "proto/scene/scene_data.pb.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

using raytracer::RendererConfig;
using raytracer::SceneConfig;

// Renderer config flags.
DEFINE_bool(shadows, true, "Whether or not shadows are rendered");

DEFINE_uint64(recursion_depth, 10, "How deep to evaluate reflective and "
                                   "refractive rays");

DEFINE_uint64(rays_per_pixel, 1, "The number of randomly jittered rays to "
                                 "shoot through each pixel");

DEFINE_bool(use_kd_tree, true, "Whether or not to use a KdTree in the scene");

DEFINE_uint64(worker_threads, 8, "Number of rendering worker threads to use");

// Camera config flags.
DEFINE_int32(image_resolution_x, -1, "Optional override for the horizontal "
                                      "resolution of the image");

DEFINE_int32(image_resolution_y, -1, "Optional override for the vertical "
                                      "resolution of the image");

// Output flags.
DEFINE_string(bmp_file, "image", "If <file> is passed, a BMP image will be "
                                  "saved at 'output/<file>.bmp'");

DEFINE_string(ppm_file, "", "If <file> is passed, a PPM image will be saved at "
                            "'output/<file>.ppm'");

DEFINE_string(scene_data, "data/scene/quadrics_tori.sd",
                          "A file from which to parse the items in the scene");

/* General Todos:

TODO(dinow): Add textures, perlin noise
TODO(dinow): Add supersampling (as preparation for global illumination)
TODO(dinow): Change update mechanism to have different kinds of updates.
TODO(dinow): Add possibility to load renderer config from file.
TODO(dinow): Rename proto namespace to "config" or "proto".
TODO(dinow): Put everything else in namespace "raytracer".
TODO(dinow): Implement a GUI and/or webserver.
TODO(dinow): Implement statistics (intersection counting).
TODO(dinow): Remove the default values from the object constructors.
             Leave them only in the protos. Or leave them in both but always
             pass the arguments.
*/

bool LoadSceneData(const std::string& path, raytracer::SceneData* output) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    return false;
  }
  std::string string((std::istreambuf_iterator<char>(stream)),
                      std::istreambuf_iterator<char>());
  return google::protobuf::TextFormat::ParseFromString(string, output);
}

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

  // Load the configuration from the passed arguments.
  SceneConfig scene_config;
  if (FLAGS_use_kd_tree) {
    scene_config.mutable_kd_tree_config();
  }
  if (FLAGS_scene_data.empty()) {
    LOG(ERROR) << "Failed to load scene data, no file provided";
    return EXIT_FAILURE;
  }
  if (LoadSceneData(FLAGS_scene_data, scene_config.mutable_scene_data())) {
    LOG(INFO) << "Loaded scene data from: " << FLAGS_scene_data;
  } else {
    LOG(ERROR) << "Failed to load scene data from: " << FLAGS_scene_data;
    return EXIT_FAILURE;
  }

  auto& camera_config = *(scene_config.mutable_scene_data()->mutable_camera());
  if (FLAGS_image_resolution_x > 0) {
    camera_config.set_resolution_x(FLAGS_image_resolution_x);
  }
  if (FLAGS_image_resolution_y > 0) {
    camera_config.set_resolution_y(FLAGS_image_resolution_y);
  }

  // Build the scene from the config.
  std::unique_ptr<Scene> scene(Scene::FromConfig(scene_config));

  // Load renderer config from flags.
  RendererConfig renderer_config;
  renderer_config.set_threads(FLAGS_worker_threads);
  renderer_config.set_shadows(FLAGS_shadows);
  renderer_config.set_recursion_depth(FLAGS_recursion_depth);
  renderer_config.set_rays_per_pixel(FLAGS_rays_per_pixel);

  // Build a renderer from the config.
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(renderer_config));

  if (!FLAGS_bmp_file.empty()) {
    renderer->AddListener(new BmpExporter("output/" + FLAGS_bmp_file + ".bmp"));
  }
  if (!FLAGS_ppm_file.empty()) {
    renderer->AddListener(new PpmExporter("output/" + FLAGS_ppm_file + ".ppm"));
  }
  renderer->AddListener(new ProgressListener());

  // Render the image.
  renderer->Render(scene.get());

  // Free all memory in the various Google libraries.
  google::protobuf::ShutdownProtobufLibrary();
  google::ShutdownGoogleLogging();
  google::ShutDownCommandLineFlags();
  return EXIT_SUCCESS;
}
