/*
 * Is in charge of rendering a scene. Runs the rendering loop and emits events.
 * Author: Dino Wernli
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include<memory>
#include<vector>

#include "util/color3.h"
#include "util/no_copy_assign.h"

class Ray;
class Sampler;
class Scene;
class Shader;
class Updatable;

namespace raytracer {
class RendererConfig;
}

class Renderer {
 public:
  // Takes ownership of all passed pointers. The argument "num_threads"
  // determines the number of worker threads in addition to the monitoring
  // thread.
  Renderer(Sampler* sampler, Shader* shader, size_t num_threads,
           size_t recursion_depth);
  virtual ~Renderer();
  NO_COPY_ASSIGN(Renderer);

  // Takes ownership of the passed listener. Does nothing is the listener has
  // already been added.
  void AddListener(Updatable* listener);

  // Starts the rendering process. Does not take ownership of the passed scene.
  void Render(Scene* scene);

  // Builds a new Renderer from configuration. The caller takes ownership of
  // the returned object.
  static Renderer* FromConfig(const raytracer::RendererConfig& config);

 private:
  // Serves as the method passed to threads. It contains the rendering loop
  // which consists of fetching samples, tracing them, and putting them back.
  void WorkerMain(size_t worker_id);

  // Traces the color of the provided ray in the scene.
  Color3 TraceColor(const Ray& ray);

  void UpdateListeners() const;

  // The renderer does not own the scene.
  Scene* scene_;

  std::unique_ptr<Sampler> sampler_;
  std::unique_ptr<Shader> shader_;
  std::vector<std::unique_ptr<Updatable> > listeners_;

  size_t num_threads_;
  size_t recursion_depth_;

  // The sleep time for the monitor thread.
  static const size_t kSleepTimeMilli;

  static const size_t kMicroToMilli;
};

#endif  /* RENDERER_H_ */
