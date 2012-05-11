/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include <glog/logging.h>

#include "parser/mesh_parser.h"
#include "parser/scene_parser.h"
#include "proto/configuration.pb.h"
#include "renderer/intersection_data.h"
#include "scene/camera.h"
#include "scene/element.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/point_light.h"
#include "scene/geometry/plane.h"
#include "scene/geometry/sphere.h"
#include "scene/geometry/triangle.h"
#include "scene/mesh.h"
#include "util/ray.h"

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
  SceneParser::ParseScene(config.scene_data(), scene);
  return scene;
}

// static
Scene* Scene::HorseScene(const raytracer::SceneConfig& config) {
  Scene* scene = FromConfig(config);

  scene->AddLight(new PointLight(Point3(10, 8, 1), Color3(1, 1, 1)));
  scene->AddLight(new PointLight(Point3(-5, 4, 7), Color3(1, 1, 1)));

  scene->set_background(Color3(0.8, 0.9, 1));
  scene->set_ambient(Color3(0.02, 0.02, 0.02));
  scene->set_camera(new Camera(Point3(6.5, 1, 3), Vector3(-2.5, -0.3, -1),
                               Vector3(0, 1, 0), 20, 500, 500));

  Color3 b(0, 0, 0);
  Material* blue = new Material(b, Color3(0, 0, 1), Color3(0.2, 0.5, 1),
                                Color3(0, 0, 0.3));
  Material* green = new Material(b, Color3(0, 0.1, 0), Color3(0, 0.5, 0),
                                 Color3(0, 0.3, 0));
  scene->AddMaterial(green);
  scene->AddMaterial(blue);

  scene->AddElement(new Plane(Point3(0, -1.5, 0), Vector3(0, 1, 0), green));

  MeshParser parser;
  Mesh* horse = parser.LoadFile("data/mesh/horse.obj");
  horse->set_material(blue);
  horse->Transform(2, Vector3(0, 0, 0));
  horse->InferNormals();
  scene->AddMesh(horse);

  return scene;
}

// static
Scene* Scene::TestScene(const raytracer::SceneConfig& config) {
  Scene* scene = FromConfig(config);

  scene->AddLight(new PointLight(Point3(10, 8, 1), Color3(1, 1, 1)));
  scene->AddLight(new PointLight(Point3(-5, 4, 7), Color3(1, 1, 1)));

  scene->set_background(Color3(0.8, 0.9, 1));
  scene->set_ambient(Color3(0.02, 0.02, 0.02));
  scene->set_camera(new Camera(Point3(2, 0.4, 1), Vector3(-2.5, -0.3, -1),
                               Vector3(0, 1, 0), 20, 500, 500));

  Color3 b(0, 0, 0);
  Material* blue = new Material(b, Color3(0, 0, 1), Color3(0.2, 0.5, 1),
                                Color3(0, 0, 0.3));
  scene->AddMaterial(blue);

  MeshParser parser;
  Mesh* horse = parser.LoadFile("data/mesh/horse.obj");
  horse->set_material(blue);
  scene->AddMesh(horse);

  return scene;
}
