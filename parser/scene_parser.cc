/*
 * Author: Dino Wernli
 */

#include "scene_parser.h"

#include <glog/logging.h>
#include <map>
#include <memory>
#include <string>

#include "proto/scene/scene_data.pb.h"
#include "proto/util/color_data.pb.h"
#include "scene/camera.h"
#include "scene/element.h"
#include "scene/geometry/plane.h"
#include "scene/geometry/sphere.h"
#include "scene/geometry/triangle.h"
#include "scene/material.h"
#include "scene/point_light.h"
#include "scene/scene.h"
#include "util/color3.h"
#include "util/point3.h"
#include "util/vector3.h"

SceneParser::SceneParser() {
}

SceneParser::~SceneParser() {
}

static Color3 Parse(const raytracer::ColorData& data) {
  return Color3(data.r(), data.g(), data.b());
}

static Vector3 Parse(const raytracer::VectorData& data) {
  return Vector3(data.x(), data.y(), data.z());
}

static Point3 Parse(const raytracer::PointData& data) {
  return Point3(data.x(), data.y(), data.z());
}

// Fetches the material pointer from the map, returns none if it is not found.
static const Material* GetMaterial(
    const std::string& id, const std::map<std::string, const Material*>& map) {
  auto it = map.find(id);
  return ((it == map.end()) ? NULL : it->second);
}

// static
void SceneParser::ParseScene(const raytracer::SceneData& data, Scene* scene) {
  // Maps material identifiers to materials.
  std::map<std::string, const Material*> material_map;

  for (int i = 0; i < data.materials_size(); ++i) {
    const auto& mat_data = data.materials(i);
    if (!(mat_data.has_emission() && mat_data.has_ambient()
        && mat_data.has_diffuse() && mat_data.has_specular()
        && mat_data.has_shininess())) {
      LOG(WARNING) << "Skipping incomplete material";
      continue;
    }

    Material* current = new Material(Parse(mat_data.emission()),
                                     Parse(mat_data.ambient()),
                                     Parse(mat_data.diffuse()),
                                     Parse(mat_data.specular()),
                                     mat_data.shininess());
    // Ownership taken by scene.
    scene->AddMaterial(current);
    material_map[mat_data.identifier()] = current;
  }

  if (data.has_camera()) {
    const auto& camera = data.camera();
    if (!(camera.has_position() && camera.has_view() && camera.has_up()
        && camera.has_opening_angle() && camera.resolution_x()
        && camera.resolution_y())) {
      LOG(WARNING) << "Skipping incomplete camera";
    } else {
      scene->set_camera(new Camera(Parse(data.camera().position()),
                                   Parse(data.camera().view()),
                                   Parse(data.camera().up()),
                                   data.camera().opening_angle(),
                                   data.camera().resolution_x(),
                                   data.camera().resolution_y()));
    }
  }

  for (int i = 0; i < data.lights_size(); ++i) {
    if (!(data.lights(i).has_position() && data.lights(i).has_color())) {
      LOG(WARNING) << "Skipping incomplete light";
      continue;
    }
    scene->AddLight(new PointLight(Parse(data.lights(i).position()),
                                   Parse(data.lights(i).color())));
  }

  for (int i = 0; i < data.element_groups_size(); ++i) {
    const auto& group = data.element_groups(i);

    // Parse triangles if any.
    for (int j = 0; j < group.triangles_size(); ++j) {
      const auto& triangle = group.triangles(j);
      std::unique_ptr<Vector3> n1(triangle.has_n1() ?
          new Vector3(Parse(triangle.n1())) : NULL);
      std::unique_ptr<Vector3> n2(triangle.has_n2() ?
          new Vector3(Parse(triangle.n2())) : NULL);
      std::unique_ptr<Vector3> n3(triangle.has_n3() ?
          new Vector3(Parse(triangle.n3())) : NULL);

      if (triangle.has_p1() && triangle.has_p2() && triangle.has_p3()) {
        scene->AddElement(
            new Triangle(Parse(triangle.p1()),
                         Parse(triangle.p2()),
                         Parse(triangle.p3()),
                         GetMaterial(triangle.material_id(), material_map),
                         n1.get(), n2.get(), n3.get()));
      } else {
        LOG(WARNING) << "Skipping incomplete triangle";
      }
    }

    // Parse planes if any.
    for (int j = 0; j < group.planes_size(); ++j) {
      const auto& plane = group.planes(j);
      if (plane.has_point() && plane.has_normal()) {
        scene->AddElement(
            new Plane(Parse(plane.point()),
                      Parse(plane.normal()),
                      GetMaterial(plane.material_id(), material_map)));
      } else {
        LOG(WARNING) << "Skipping incomplete plane";
      }
    }

    // Parse spheres if any.
    for (int j = 0; j < group.spheres_size(); ++j) {
      const auto& sphere = group.spheres(j);
      if (sphere.has_center() && sphere.has_radius()) {
        scene->AddElement(new Sphere(Parse(sphere.center()),
                                     sphere.radius(),
                                     GetMaterial(sphere.material_id(),
                                                 material_map)));
      } else {
        LOG(WARNING) << "Skipping incomplete sphere";
      }
    }
  }
}
