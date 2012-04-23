/*
 *	Author: Dino Wernli
 */

#include "Scene.h"

#include "scene/Element.h"

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::AddElement(Element* element) {
  elements_.push_back(std::unique_ptr<Element>(element));
}
