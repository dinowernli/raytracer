/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include "renderer/intersection_data.h"
#include "scene/element.h"
#include "scene/light.h"
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
