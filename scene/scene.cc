/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include <glog/logging.h>

#include "parser/scene_parser.h"
#include "proto/config/scene_config.pb.h"
#include "scene/element.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/mesh.h"

Scene::Scene(KdTree* kd_tree)
    : kd_tree_(kd_tree), background_(Color3(1, 1, 1)),
      ambient_(Color3(0, 0, 0)), refraction_index_(1) {
}

Scene::~Scene() {
}

void Scene::AddElement(Element* element) {
  elements_.push_back(std::unique_ptr<Element>(element));
}

void Scene::AddLight(Light* light) {
  lights_.push_back(std::unique_ptr<Light>(light));
}

void Scene::AddMaterial(Material* material) {
  materials_.push_back(std::unique_ptr<Material>(material));
}

void Scene::AddMesh(Mesh* mesh) {
  meshes_.push_back(std::unique_ptr<Mesh>(mesh));
  mesh->CreateElements(&elements_);
}

void Scene::Init() {
  DVLOG(1) << "Initializing scene with " << elements_.size() << " elements";
  if(UsesKdTree()) {
    kd_tree_->Init(elements_);
  }
  LOG(INFO) << "Scene initialized";
}

bool Scene::Intersect(const Ray& ray, IntersectionData* data) const {
  if(UsesKdTree()) {
    return kd_tree_->Intersect(ray, data);
  } else {
    bool result = false;
    for (auto it = elements_.begin(); it != elements_.end(); ++it) {
      result = it->get()->Intersect(ray, data) | result;
      if (result && data == NULL) {
        return true;
      }
    }
    return result;
  }
}

// static
Scene* Scene::FromConfig(const raytracer::SceneConfig& config) {
  KdTree* tree = config.has_kd_tree_config() ? new KdTree() : NULL;
  Scene* scene = new Scene(tree);
  SceneParser parser;
  parser.ParseScene(config.scene_data(), scene);
  return scene;
}
