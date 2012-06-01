/*
 * Author: Dino Wernli
 */

#include "scene_parser.h"

#include <glog/logging.h>

#include "parser/mesh_parser.h"
#include "proto/scene/scene_data.pb.h"
#include "proto/util/color_data.pb.h"
#include "scene/camera.h"
#include "scene/element.h"
#include "scene/geometry/circle_plane.h"
#include "scene/geometry/plane.h"
#include "scene/geometry/sphere.h"
#include "scene/geometry/triangle.h"
#include "scene/light/point_light.h"
#include "scene/light/sphere_light.h"
#include "scene/material.h"
#include "scene/mesh.h"
#include "scene/scene.h"

SceneParser::SceneParser() {
}

SceneParser::~SceneParser() {
}

// static
Color3 SceneParser::Parse(const raytracer::ColorData& data) {
  return Color3(data.r(), data.g(), data.b());
}

// static
Vector3 SceneParser::Parse(const raytracer::VectorData& data) {
  return Vector3(data.x(), data.y(), data.z());
}

// static
Point3 SceneParser::Parse(const raytracer::PointData& data) {
  return Point3(data.x(), data.y(), data.z());
}

// static
Material* SceneParser::Parse(const raytracer::MaterialData& data) {
  if (!(data.has_emission() && data.has_ambient()
      && data.has_diffuse() && data.has_specular())) {
    return NULL;
  }

  return new Material(Parse(data.emission()), Parse(data.ambient()),
                      Parse(data.diffuse()), Parse(data.specular()),
                      data.shininess(), data.reflection_percentage(),
                      data.refraction_percentage(), data.refraction_index());
}

void SceneParser::ParseScene(const raytracer::SceneData& data, Scene* scene) {
  material_map_.clear();

  scene->set_background(Parse(data.background()));
  scene->set_ambient(Parse(data.ambient()));
  scene->set_refraction_index(data.refraction_index());

  Material* material = NULL;
  for (int i = 0; i < data.materials_size(); ++i) {
    if (!data.materials(i).has_identifier()) {
      LOG(WARNING) << "Skipping material without identifier";
      continue;
    }

    if (!(material = Parse(data.materials(i)))) {
      LOG(WARNING) << "Skipping incomplete material";
      continue;
    }

    // Ownership taken by scene.
    scene->AddMaterial(material);
    material_map_[data.materials(i).identifier()] = material;
  }

  if (data.has_camera()) {
    const auto& camera = data.camera();
    if (!(camera.has_position() && camera.has_view() && camera.has_up()
        && camera.has_opening_angle() && camera.resolution_x()
        && camera.resolution_y())) {
      LOG(WARNING) << "Skipping incomplete camera";
    } else {
      auto& dof = data.camera().depth_of_field();
      scene->set_camera(new Camera(Parse(data.camera().position()),
                                   Parse(data.camera().view()),
                                   Parse(data.camera().up()),
                                   data.camera().opening_angle(),
                                   data.camera().resolution_x(),
                                   data.camera().resolution_y(),
                                   dof.focal_depth(), dof.lens_size()));
    }
  }

  for (int i = 0; i < data.point_lights_size(); ++i) {
    auto& l = data.point_lights(i);
    if (!(l.has_position() && l.has_color())) {
      LOG(WARNING) << "Skipping incomplete point light";
      continue;
    }
    scene->AddLight(new PointLight(Parse(l.position()), Parse(l.color())));
  }

  for (int i = 0; i < data.sphere_lights_size(); ++i) {
    auto& l = data.sphere_lights(i);
    if (!(l.has_center() && l.has_color() && l.has_radius())) {
      LOG(WARNING) << "Skipping incomplete sphere light";
      continue;
    }
    scene->AddLight(new SphereLight(Parse(l.center()), l.radius(),
                                    Parse(l.color())));
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
      if (!(material = GetMaterial(triangle.material_id()))) {
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
      if (!(material = GetMaterial(plane.material_id()))) {
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
    if (!(main_material = GetMaterial(cplane.plane_data().material_id()))) {
      LOG(WARNING) << "Failed to get main material, skipping circle plane";
      continue;
    }

    const Material* ring_material;
    if (!(ring_material = GetMaterial(cplane.ring_material_id()))) {
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
    if (!(material = GetMaterial(sphere.material_id()))) {
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
        if (!(material = GetMaterial(mesh_data.material_id()))) {
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

// Fetches the material pointer from the map, returns none if it is not found.
Material* SceneParser::GetMaterial(const std::string& id) const {
  auto it = material_map_.find(id);
  return ((it == material_map_.end()) ? NULL : it->second);
}
