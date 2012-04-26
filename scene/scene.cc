/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include "scene/element.h"
#include "scene/light.h"

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
