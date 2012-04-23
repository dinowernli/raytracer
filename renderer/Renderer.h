/*
 * Is in charge of rendering a scene. Runs the rendering loop and emits events.
 * Author: Dino Wernli
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include<memory>

class Scene;

namespace raytracer {
class Configuration;
}

class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  // Builds a new Renderer from configuration. The caller takes ownership of
  // the returned object.
  static Renderer* FromConfig(const raytracer::Configuration& config);

 private:
  std::unique_ptr<Scene> scene_;
};

#endif  /* RENDERER_H_ */
