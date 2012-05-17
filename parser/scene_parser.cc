/*
 * Author: Dino Wernli
 */

#include "scene_parser.h"

#include <glog/logging.h>
#include <map>
#include <memory>
#include <string>

#include "parser/mesh_parser.h"
#include "proto/scene/scene_data.pb.h"
#include "proto/util/color_data.pb.h"
#include "scene/camera.h"
#include "scene/element.h"
#include "scene/geometry/circle_plane.h"
#include "scene/geometry/plane.h"
#include "scene/geometry/sphere.h"
#include "scene/geometry/triangle.h"
#include "scene/material.h"
#include "scene/mesh.h"
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

  if (data.has_background()) {
    scene->set_background(Parse(data.background()));
  }

  if (data.has_ambient()) {
    scene->set_ambient(Parse(data.ambient()));
  }

  for (int i = 0; i < data.materials_size(); ++i) {
    const auto& mat_data = data.materials(i);
    if (!(mat_data.has_emission() && mat_data.has_ambient()
        && mat_data.has_diffuse() && mat_data.has_specular())) {
      LOG(WARNING) << "Skipping incomplete material";
      continue;
    }

    Material* current = new Material(Parse(mat_data.emission()),
                                     Parse(mat_data.ambient()),
                                     Parse(mat_data.diffuse()),
                                     Parse(mat_data.specular()),
                                     mat_data.shininess(),
                                     mat_data.reflection_percentage(),
                                     mat_data.refraction_percentage(),
                                     mat_data.refraction_index());
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

  // Parse triangles if any.
  for (int i = 0; i < data.triangles_size(); ++i) {
    const auto& triangle = data.triangles(i);
    std::unique_ptr<Vector3> n1(triangle.has_n1() ?
        new Vector3(Parse(triangle.n1())) : NULL);
    std::unique_ptr<Vector3> n2(triangle.has_n2() ?
        new Vector3(Parse(triangle.n2())) : NULL);
    std::unique_ptr<Vector3> n3(triangle.has_n3() ?
        new Vector3(Parse(triangle.n3())) : NULL);

    if (triangle.has_p1() && triangle.has_p2() && triangle.has_p3()) {
      const Material* material;
      if (!(material = GetMaterial(triangle.material_id(), material_map))) {
        LOG(WARNING) << "Failed to get material, skipping triangle";
        continue;
      }
      scene->AddElement(
          new Triangle(Parse(triangle.p1()), Parse(triangle.p2()),
                       Parse(triangle.p3()), *material,
                       n1.get(), n2.get(), n3.get()));
    } else {
      LOG(WARNING) << "Skipping incomplete triangle";
    }
  }

  // Parse planes if any.
  for (int i = 0; i < data.planes_size(); ++i) {
    const auto& plane = data.planes(i);
    if (plane.has_point() && plane.has_normal()) {
      const Material* material;
      if (!(material = GetMaterial(plane.material_id(), material_map))) {
        LOG(WARNING) << "Failed to get material, skipping plane";
        continue;
      }

      scene->AddElement(
          new Plane(Parse(plane.point()), Parse(plane.normal()), *material));
    } else {
      LOG(WARNING) << "Skipping incomplete plane";
    }
  }

  // Parse circle planes if any.
  for (int i = 0; i < data.circle_planes_size(); ++i) {
    const auto& cplane = data.circle_planes(i);
    if (!(cplane.has_plane_data() && cplane.plane_data().has_point()
          && cplane.plane_data().has_normal())) {
      LOG(WARNING) << "Skipping incomplete circle plane";
      continue;
    }

    const Material* main_material;
    if (!(main_material = GetMaterial(cplane.plane_data().material_id(),
                                      material_map))) {
      LOG(WARNING) << "Failed to get main material, skipping circle plane";
      continue;
    }

    const Material* ring_material;
    if (!(ring_material = GetMaterial(cplane.ring_material_id(),
                                      material_map))) {
      LOG(WARNING) << "Failed to get ring material, skipping circle plane";
      continue;
    }

    scene->AddElement(new CirclePlane(
        Parse(cplane.plane_data().point()), Parse(cplane.plane_data().normal()),
        cplane.radius(), *main_material, *ring_material));
  }

  // Parse spheres if any.
  for (int i = 0; i < data.spheres_size(); ++i) {
    const auto& sphere = data.spheres(i);

    const Material* material;
    if (!(material = GetMaterial(sphere.material_id(), material_map))) {
      LOG(WARNING) << "Failed to get material, skipping sphere";
      continue;
    }

    if (sphere.has_center() && sphere.has_radius()) {
      scene->AddElement(new Sphere(Parse(sphere.center()), sphere.radius(),
                                   *material));
    } else {
      LOG(WARNING) << "Skipping incomplete sphere";
    }
  }

  // Load meshes if any.
  MeshParser parser;
  for (int i = 0; i < data.meshes_size(); ++i) {
    const auto& mesh_data = data.meshes(i);
    if (mesh_data.has_obj_file()) {
      const std::string& path = mesh_data.obj_file();
      Mesh* mesh = parser.LoadFile(path);
      if (mesh != NULL) {
        const Material* material;
        if (!(material = GetMaterial(mesh_data.material_id(), material_map))) {
          LOG(WARNING) << "Failed to get main material, skipping mesh";
          continue;
        }

        mesh->set_material(material);
        if (mesh_data.has_translation() || mesh_data.has_radius()) {
          Vector3 translation;
          if (mesh_data.has_translation()) {
            translation = Parse(mesh_data.translation());
          }
          mesh->Transform(mesh_data.has_radius() ? mesh_data.radius() : 1,
                          translation);
        }
        mesh->InferNormals();
        scene->AddMesh(mesh);
      } else {
        LOG(WARNING) << "Unable to load mesh from: " << path;
      }
    } else {
      LOG(WARNING) << "Skipping incomplete mesh";
    }
  }
}
