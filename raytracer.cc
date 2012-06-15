/*
 * A command line binary for the ray tracer.
 * Autor: Dino Wernli
 */

#include <fstream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/text_format.h>
#include <memory>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>

#include "listener/bmp_exporter.h"
#include "listener/ppm_exporter.h"
#include "listener/progress_listener.h"
#include "listener/raytracer_window.h"
#include "proto/config/renderer_config.pb.h"
#include "proto/config/scene_config.pb.h"
#include "proto/scene/scene_data.pb.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

using raytracer::RendererConfig;
using raytracer::SceneConfig;
using std::string;

// Renderer config flags.
DEFINE_bool(shadows, true, "Whether or not shadows are rendered");

DEFINE_uint64(recursion_depth, 10, "How deep to evaluate reflective and "
                                   "refractive rays");

DEFINE_int32(root_rays_per_pixel, 1, "The side length of the supersampling "
                                      " square.");

DEFINE_bool(use_kd_tree, true, "Whether or not to use a KdTree in the scene");

DEFINE_int32(kd_tree_visualization_depth, -1, "How deep in the tree to "
                                              "visualize splitting planes");

DEFINE_bool(dof_visualization, false, "If true and the camera has a depth of"
                                       " field, adds a sphere to scene which "
                                       "shown the focal depth.");

DEFINE_uint64(worker_threads, 8, "Number of rendering worker threads to use");

DEFINE_string(sampler_type, "", "The type of sampler to use. Legal values are "
                                "'scanline' and 'progressive'");

DEFINE_string(splitting_strategy, "", "The strategy to use for splitting in the"
                                      " KdTree. Only has effect if use_kd_tree "
                                      "is true. Legal values are 'midpoint'");

DEFINE_double(adaptive_supersampling_threshold, -1, "A threshold for the "
                                                    "variance in adaptive "
                                                    "supersampling.");

// Camera config flags.
DEFINE_int32(image_resolution_x, -1, "Optional override for the horizontal "
                                      "resolution of the image");

DEFINE_int32(image_resolution_y, -1, "Optional override for the vertical "
                                      "resolution of the image");

DEFINE_double(dof_lens_size, -1, "The size of the depth-of-field camera lens."
                                 " A good value is about 0.1.");

DEFINE_double(dof_focal_depth, -1, "The depth-of-field focal depth of the "
                                  "camera lens.");

// Output flags.
DEFINE_string(bmp_file, "", "If <file> is passed, a BMP image will be saved at "
                            "'output/<file>.bmp'. Otherwise, a default name "
                            "is inferred from the config.");

DEFINE_string(ppm_file, "", "If <file> is passed, a PPM image will be saved at "
                            "'output/<file>.ppm'");

DEFINE_string(scene_data, "data/scene/quadrics_tori.sd",
                          "A file from which to parse the items in the scene");

DEFINE_bool(gui, false, "Whether or not to start the GLUT front end");

/* General Todos:

TODO(dinow): Add support for building and running binaries and single tests in
             scons. Add multiple targets.
TODO(dinow): Implement surface area heuristics for KdTree splitting
TODO(dinow): Implement adaptive supersampling
TODO(dinow): Add textures, perlin noise, bump maps, skyboxes, path tracing
TODO(dinow): Add possibility to load renderer config from file.
TODO(dinow): Rename proto namespace to "config" or "proto".
TODO(dinow): Put everything else in namespace "raytracer".
TODO(dinow): Implement a webserver.
TODO(dinow): Implement statistics (intersection counting).
TODO(dinow): Remove the default values from the object constructors.
             Leave them only in the protos. Or leave them in both but always
             pass the arguments.
*/

string DefaultFilename() {
  std::stringstream stream;
  stream << FLAGS_scene_data.substr(FLAGS_scene_data.find_last_of('/') + 1)
         << "_rrpp" << FLAGS_root_rays_per_pixel;
  return stream.str();
}

bool LoadSceneData(const string& path, raytracer::SceneData* output) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    return false;
  }
  string string((std::istreambuf_iterator<char>(stream)),
                      std::istreambuf_iterator<char>());
  return google::protobuf::TextFormat::ParseFromString(string, output);
}

int main(int argc, char **argv) {
  // LOG(INFO): Always logged.
  // DVLOG(i): Only compiled in if DEBUG flag set.
  //
  // Run <binary name> --logtostderr to see log output.
  // Run <binary name> --logtostderr --v=<i> for DVLOG(j) message for j <= i.
  google::InitGoogleLogging(argv[0]);
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

  if (!FLAGS_splitting_strategy.empty()) {
    if (FLAGS_splitting_strategy == "midpoint") {
      scene_config.mutable_kd_tree_config()
          ->set_splitting_strategy(raytracer::KdTreeConfig::MIDPOINT);
    } else {
      LOG(WARNING) << "Skipping unknown splitting strategy: "
                   << FLAGS_splitting_strategy;
    }
  }

  // TODO(dinow): Add support for color from flags.
  raytracer::ColorData vis_color;
  vis_color.set_r(0.6); vis_color.set_g(0.25); vis_color.set_b(0.1);

  if (FLAGS_dof_lens_size >= 0 && FLAGS_dof_focal_depth >= 0) {
    auto* camera = scene_config.mutable_scene_data()->mutable_camera();
    auto* dof = camera->mutable_depth_of_field();
    dof->set_lens_size(FLAGS_dof_lens_size);
    dof->set_focal_depth(FLAGS_dof_focal_depth);
    if(FLAGS_dof_visualization) {
      dof->mutable_visualization_color()->CopyFrom(vis_color);
    }
  }

  if (FLAGS_use_kd_tree && FLAGS_kd_tree_visualization_depth >= 0) {
    raytracer::KdTreeConfig* kd_config = scene_config.mutable_kd_tree_config();
    kd_config->set_visualization_depth(FLAGS_kd_tree_visualization_depth);
    kd_config->mutable_visualization_color()->CopyFrom(vis_color);
  }

  // Build the scene from the config.
  std::unique_ptr<Scene> scene(Scene::FromConfig(scene_config));

  // Load renderer config from flags.
  RendererConfig renderer_config;
  renderer_config.set_threads(FLAGS_worker_threads);
  renderer_config.set_shadows(FLAGS_shadows);
  renderer_config.set_recursion_depth(FLAGS_recursion_depth);
  renderer_config.set_root_rays_per_pixel(FLAGS_root_rays_per_pixel);
  renderer_config.set_adaptive_supersampling_threshold(
      FLAGS_adaptive_supersampling_threshold);

  if (!FLAGS_sampler_type.empty()) {
    if (FLAGS_sampler_type == "progressive") {
      renderer_config.set_sampler_type(RendererConfig::PROGRESSIVE);
    } else if (FLAGS_sampler_type == "scanline") {
      renderer_config.set_sampler_type(RendererConfig::SCANLINE);
    } else {
      LOG(WARNING) << "Skipping unknown sampler type: " << FLAGS_sampler_type;
    }
  }

  // Build a renderer from the config.
  std::unique_ptr<Renderer> renderer(Renderer::FromConfig(renderer_config));

  renderer->AddListener(new ProgressListener());
  const string dir = "output/";
  if (!FLAGS_bmp_file.empty()) {
    renderer->AddListener(new BmpExporter(dir + FLAGS_bmp_file + ".bmp"));
  } else {
    renderer->AddListener(new BmpExporter(dir + DefaultFilename() + ".bmp"));
  }

  if (!FLAGS_ppm_file.empty()) {
    renderer->AddListener(new PpmExporter("output/" + FLAGS_ppm_file + ".ppm"));
  }

  RaytracerWindow* window = NULL;
  if (FLAGS_gui) {
    window = new RaytracerWindow(&argc, argv);
    renderer->AddListener(window);
  }

  // Run the rendering itself on an own thread to allow the UI (if any) to be
  // on the main thread.
  std::thread thread(&Renderer::Render, renderer.get(), scene.get());

  if (window != NULL) {
    // This never returns. That's ok since memory freeing is done after this
    // exits anyway.
    window->MainLoop();
  }

  // Only relevant if there is no GUI.
  thread.join();

  // Free all memory in the various Google libraries.
  google::protobuf::ShutdownProtobufLibrary();
  google::ShutdownGoogleLogging();
  google::ShutDownCommandLineFlags();
  return EXIT_SUCCESS;
}
