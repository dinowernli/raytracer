/*
 * Contains elements, lights etc. and can be rendered.
 * Author: Dino Wernli
 */

#ifndef SCENE_H_
#define SCENE_H_

#include<memory>
#include<vector>

#include "scene/camera.h"
#include "util/no_copy_assign.h"

class Element;
class Image;
class Light;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  NO_COPY_ASSIGN(Scene);

  // Takes ownership of the passed element.
  void AddElement(Element* element);

  // Takes ownership of the pass light.
  void AddLight(Light* light);

  // Takes ownership of the passed camera.
  void set_camera(Camera* camera) { camera_.reset(camera); }

  // Constructs the image, builds data structures etc.
  void Init();

 private:
  std::vector<std::unique_ptr<Element> > elements_;
  std::vector<std::unique_ptr<Light> > lights_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Image> image_;
};

#endif  /* SCENE_H_ */
