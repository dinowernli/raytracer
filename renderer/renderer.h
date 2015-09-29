// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
class Statistics;
class Supersampler;
class Updatable;

namespace raytracer {
class RendererConfig;
}

class Renderer {
 public:
  // Takes ownership of all passed pointers. The argument "num_threads"
  // determines the number of worker threads in addition to the monitoring
  // thread.
  Renderer(Sampler* sampler, Supersampler* supersampler, Shader* shader,
           size_t num_threads, size_t recursion_depth, Statistics* stats);
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

  bool HasStatistics() const { return statistics_.get() != NULL; }
  const Statistics& statistics() const { return *statistics_; }

 private:
  // Serves as the method passed to threads. It contains the rendering loop
  // which consists of fetching samples, tracing them, and putting them back.
  void WorkerMain(size_t worker_id);

  // Traces the color of the provided ray in the scene. The argument depth
  // indicates the current depth of the recursion.
  Color3 TraceColor(const Ray& ray, size_t depth,
                    std::vector<Scalar>* refraction_stack);

  // The renderer does not own the scene.
  Scene* scene_;

  std::unique_ptr<Sampler> sampler_;
  std::unique_ptr<Supersampler> supersampler_;
  std::unique_ptr<Shader> shader_;
  std::vector<std::unique_ptr<Updatable>> listeners_;

  // Stores the number of worker threads besides the controller thread.
  size_t num_threads_;

  // Stores the depth to which reflection and refraction are evaluated.
  size_t recursion_depth_;

  std::unique_ptr<Statistics> statistics_;

  // The sleep time for the monitor thread.
  static const size_t kSleepTimeMilli;
};

#endif  /* RENDERER_H_ */
