/*
 * Author: Dino Wernli
 */

#include "Scene.h"

#include "renderer/Image.h"
#include "scene/Element.h"
#include "scene/Light.h"

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
  // TODO(dinow): Handle missing camera.
  image_.reset(new Image(camera_->resolution_x(), camera_->resolution_y()));
}
