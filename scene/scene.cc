/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include <glog/logging.h>

#include "parser/scene_parser.h"
#include "proto/config/scene_config.pb.h"
#include "scene/element.h"
#include "scene/light/light.h"
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
    kd_tree_->Init(&elements_);
  }
  LOG(INFO) << "Scene initialized";
}

bool Scene::Intersect(const Ray& ray, IntersectionData* data) const {
  bool result = false;
  for (size_t i = 0; i < lights_.size(); ++i) {
    result = lights_[i]->Intersect(ray, data) || result;
    if (result && (data == NULL)) {
      // TODO(dinow): Check if this works with shadow rays, a shadow ray which
      // hits a light source should not be counted as occluded.
      return true;
    }
  }

  if(UsesKdTree()) {
    result = kd_tree_->Intersect(ray, data) || result;
  } else {
    for (auto it = elements_.begin(); it != elements_.end(); ++it) {
      result = it->get()->Intersect(ray, data) || result;
      if (result && (data == NULL)) {
        return true;
      }
    }
  }
  return result;
}

// static
Scene* Scene::FromConfig(const raytracer::SceneConfig& config) {
  SceneParser parser;

  KdTree* tree = NULL;
  Material* v_mat = NULL;

  if (config.has_kd_tree_config()) {
    const raytracer::KdTreeConfig kd = config.kd_tree_config();
    if (kd.splitting_strategy() == raytracer::KdTreeConfig::MIDPOINT) {
      int v_depth = kd.visualization_depth();
      if (v_depth < 0) {
        // Passing NULL as material is ok since it will never be used.
        tree = new KdTree(new MidpointSplit(), v_depth, NULL);
      } else {
        // Attempt to fetch the material.
        if (!kd.has_visualization_material() ||
            !(v_mat = parser.ParseMaterial(kd.visualization_material()))) {
          LOG(WARNING) << "Could not load KdTree visualization material, "
                       << "deactivating visualization";
          tree = new KdTree(new MidpointSplit(), -1, NULL);
        } else {
          // Visualization parsed successfully.
          tree = new KdTree(new MidpointSplit(), v_depth, v_mat);
        }
      }
    } else {
      LOG(WARNING) << "Unknown KdTree splitting strategy, skipping KdTree";
    }
  }

  Scene* scene = new Scene(tree);
  parser.ParseScene(config.scene_data(), scene);
  if (v_mat != NULL) {
    scene->AddMaterial(v_mat);
  }

  return scene;
}
