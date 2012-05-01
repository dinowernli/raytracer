/*
 * Author: Dino Wernli
 */

#include "scene.h"

#include <glog/logging.h>

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

Scene::Scene() : background_(Color3(1, 1, 1)), ambient_(Color3(0, 0, 0)),
                 refraction_index_(1) {
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
  // TODO(dinow): Build KD-tree.
  DVLOG(1) << "Scene initialized.";
}

bool Scene::Intersect(const Ray& ray, IntersectionData* data) const {
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

void Scene::GetNonOccludedLights(const Point3& position,
                                 std::vector<const Light*>* lights) const {
  for(auto it = lights_.begin(); it != lights_.end(); ++it) {
    Ray ray = it->get()->GenerateRay(position);
    if (!Intersect(ray)) {
      lights->push_back(it->get());
    }
  }
}

// static
Scene* Scene::BuildStandardScene() {
  Color3 black(0, 0, 0);
  Color3 w(1, 1, 1);
  Color3 b(0, 0, 1);
  Color3 y(1, 1, 0);
  Material* red = new Material(black, Color3(0.06, 0, 0),
                               Color3(0.6, 0, 0), Color3(1, 0.7, 0.7), 10);
  Material* green = new Material(black, Color3(0.04, 0.08, 0.1),
                                 Color3(0.4, 0.8, 0.1), Color3(0.6, 1, 0.5),
                                 20);
  Material* blue = new Material(black, b/10, b, b, 7);
  Material* yellow = new Material(black, y/10, y, y, 5);
  Material* white = new Material(black, w/10, w, w, 3);

  Scene* scene = new Scene();
  scene->AddMaterial(red);
  scene->AddMaterial(green);
  scene->AddMaterial(blue);
  scene->AddMaterial(yellow);
  scene->AddMaterial(white);

  scene->set_background(Color3(1, 0.9, 0.9));
  scene->set_camera(new Camera(Point3(1, 1, -3), Vector3(0, 0, 1),
                               Vector3(0, 1, 0), 20, 500, 500));
  scene->AddLight(new PointLight(Point3(0, 4, -3), Color3(1, 1, 1)));
  scene->AddLight(new PointLight(Point3(3, 2, 1), Color3(1, 1, 1)));
  scene->AddElement(new Sphere(Point3(0.2, 1.3, 4), 1.1, green));
  scene->AddElement(new Sphere(Point3(1.8, 1.8, 1.8), 0.5, green));
  scene->AddElement(new Plane(Point3(3, 0, 0), Vector3(-1.2, 0.2, 0), red));

  // TODO(dinow): Replace this with a CirclePlane once implemented.
  scene->AddElement(new Plane(Point3(0, 0, 0), Vector3(0, 1, 0), red));

  Point3 common(1.8, 1.3, 1.8);
  Vector3 normal(1.69, 0.6, -1.82);
  scene->AddElement(new Triangle(common, Point3(1.4, 0, 1), Point3(2.8, 0, 2.3),
                                 blue, &normal, &normal, &normal));

  normal = Vector3(0.91, 1.6, 2.34);
  scene->AddElement(new Triangle(common, Point3(2.8, 0, 2.3), Point3(1, 0, 3),
                                 blue, &normal, &normal, &normal));

  normal = Vector3(-2.6, 1.12, -0.52);
  scene->AddElement(new Triangle(common, Point3(1, 0, 3), Point3(1.4, 0, 1),
                                 blue, &normal, &normal, &normal));

  return scene;
}
