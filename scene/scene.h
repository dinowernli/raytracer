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
class IntersectionData;
class Light;
class Ray;

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
  const Camera& camera() const { return *camera_; }

  // Constructs the image, builds data structures etc. Must be called before
  // before querying for intersections. If anything is added to the scene after
  // a call to Init(), it might be ignored until the next Init() call.
  void Init();

  bool Intersect(const Ray& ray, IntersectionData* data);

 private:
  std::vector<std::unique_ptr<Element> > elements_;
  std::vector<std::unique_ptr<Light> > lights_;
  std::unique_ptr<Camera> camera_;
};

#endif  /* SCENE_H_ */
