/*
 * Contains elements, lights etc. and can be rendered.
 * Author: Dino Wernli
 */

#ifndef SCENE_H_
#define SCENE_H_

#include<memory>
#include<vector>

#include "scene/camera.h"
#include "util/color3.h"
#include "util/no_copy_assign.h"

class Element;
class IntersectionData;
class Light;
class Material;
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

  // Takes ownership of the passed material.
  void AddMaterial(Material* material);

  // Takes ownership of the passed camera.
  void set_camera(Camera* camera) { camera_.reset(camera); }
  const Camera& camera() const { return *camera_; }
  void set_background(const Material* background) { background_ = background; }
  const Material* background() const { return background_; }

  // Prepares the scene, builds data structures etc. Must be called before
  // before querying for intersections. If anything is added to the scene after
  // a call to Init(), it might be ignored until the next Init() call.
  void Init();

  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  // Add all the lights to "lights" provided they are visible from "position".
  void GetNonOccludedLights(const Point3& position,
                            std::vector<const Light*>* lights) const;

  // Builds a standard test scene. The caller takes ownership of the pointer.
  static Scene* BuildStandardScene();

 private:
  std::vector<std::unique_ptr<Element> > elements_;
  std::vector<std::unique_ptr<Light> > lights_;
  std::unique_ptr<Camera> camera_;
  std::vector<std::unique_ptr<Material> > materials_;

  // No ownership of this pointer, it is a pointer to one of the "materials".
  const Material* background_;
};

#endif  /* SCENE_H_ */
