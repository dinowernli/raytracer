/*
 * Is in charge of rendering a scene. Runs the rendering loop and emits events.
 * Author: Dino Wernli
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include<memory>
#include<vector>

#include "util/no_copy_assign.h"

class Scene;
class Updatable;

namespace raytracer {
class Configuration;
}

class Renderer {
 public:
  Renderer();
  virtual ~Renderer();
  NO_COPY_ASSIGN(Renderer);

  // Takes ownership of the passed listener. Does nothing is the listener has
  // already been added.
  void AddListener(Updatable* listener);

  // Builds a new Renderer from configuration. The caller takes ownership of
  // the returned object.
  static Renderer* FromConfig(const raytracer::Configuration& config);

 private:
  std::unique_ptr<Scene> scene_;
  std::vector<std::unique_ptr<Updatable> > listeners_;
};

#endif  /* RENDERER_H_ */
