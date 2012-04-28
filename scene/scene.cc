/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include "renderer/intersection_data.h"
#include "scene/camera.h"
#include "scene/element.h"
#include "scene/light.h"
#include "scene/point_light.h"
#include "scene/geometry/plane.h"
#include "scene/geometry/sphere.h"
#include "util/ray.h"

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::AddElement(Element* element) {
  elements_.push_back(std::unique_ptr<Element>(element));
}

void Scene::AddLight(Light* light) {
  lights_.push_back(std::unique_ptr<Light>(light));
}

void Scene::Init() {
  // TODO(dinow): Build KD-tree.
}

bool Scene::Intersect(const Ray& ray, IntersectionData* data) {
  bool result = false;
  for (auto it = elements_.begin(); it != elements_.end(); ++it) {
    result = result | it->get()->Intersect(ray, data);

    // Only return early if data is irrelevant.
    if (result && data == NULL) {
      return true;
    }
  }
  return result;
}

// static
Scene* Scene::BuildStandardScene() {
  Scene* scene = new Scene();
  scene->set_camera(new Camera(Point3(1, 1, -3), Vector3(0, 0, 1),
                               Vector3(0, 1, 0), 20, 100, 100));
  scene->AddLight(new PointLight(Point3(0, 4, -3), Color3(1, 1, 1)));
  scene->AddLight(new PointLight(Point3(3, 2, 1), Color3(1, 1, 1)));
  scene->AddElement(new Sphere(Point3(0.2, 1.3, 4), 1.1));
  scene->AddElement(new Plane(Point3(3, 0, 0), Vector3(-1.2, 0.2, 0)));

  // TODO(dinow): Replace this with a CirclePlane once implemented.
  scene->AddElement(new Plane(Point3(0, 0, 0), Vector3(0, 1, 0)));

  return scene;
}
