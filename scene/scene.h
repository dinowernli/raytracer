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
class Mesh;
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

  // Takes ownership of the passed mesh. Extracts all elements of the mesh and
  // adds them to the list of elements.
  void AddMesh(Mesh* mesh);

  // Takes ownership of the passed camera.
  void set_camera(Camera* camera) { camera_.reset(camera); }
  const Camera& camera() const { return *camera_; }
  void set_background(const Color3& background) { background_ = background; }
  const Color3& background() const { return background_; }
  void set_ambient(const Color3& ambient) { ambient_ = ambient; }
  const Color3& ambient() const { return ambient_; }
  void set_refraction_index(Scalar index) { refraction_index_ = index; }
  Scalar refraction_index() const { return refraction_index_; }

  // Prepares the scene, builds data structures etc. Must be called before
  // before querying for intersections. If anything is added to the scene after
  // a call to Init(), it might be ignored until the next Init() call.
  void Init();

  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  // Add all the lights to "lights" provided they are visible from "position".
  void GetNonOccludedLights(const Point3& position,
                            std::vector<const Light*>* lights) const;

  // Builds the Quadrics scene. The caller takes ownership of the pointer.
  static Scene* QuadricsScene();

  // Builds the Horse scene. The caller takes ownership of the pointer.
  static Scene* HorseScene();

  // Builds a test scene. The caller takes ownership of the pointer.
  static Scene* TestScene();

 private:
  std::vector<std::unique_ptr<Element>> elements_;
  std::vector<std::unique_ptr<Light>> lights_;
  std::unique_ptr<Camera> camera_;
  std::vector<std::unique_ptr<Material>> materials_;
  std::vector<std::unique_ptr<Mesh>> meshes_;

  Color3 background_;
  Color3 ambient_;
  Scalar refraction_index_;
};

#endif  /* SCENE_H_ */
