/*
 * Author: Dino Wernli
 */

#include "phong_shader.h"

#include <cmath>
#include <glog/logging.h>
#include <memory>

#include "renderer/intersection_data.h"
#include "scene/material.h"
#include "scene/point_light.h"
#include "scene/scene.h"

PhongShader::PhongShader(bool shadows) : shadows_(shadows) {
}

PhongShader::~PhongShader() {
}

static inline bool IsOccluded(const Scene& scene, const Light& light,
                              const Point3& position) {
  return scene.Intersect(light.GenerateRay(position));
}

Color3 PhongShader::Shade(const IntersectionData& data, const Scene& scene) {
  const Material& material = *data.material;
  Color3 emission(material.emission().Clamped());
  Color3 ambient((material.ambient() * scene.ambient()).Clamped());
  Color3 diffuse(0, 0, 0);
  Color3 specular(0, 0, 0);

  const std::vector<std::unique_ptr<Light>>& lights = scene.lights();
  for (auto it = lights.begin(); it != lights.end(); ++it) {
    const Light* light = it->get();
    if (shadows_ && IsOccluded(scene, *light, data.position)) {
      continue;
    }

    // TODO(dinow): Hack since we only have point lights anyway. Eventually
    // replace this by general light sampling infrastructure, which for point
    // lights just generates a single ray.
    const PointLight* p_light = dynamic_cast<const PointLight *>(light);
    CHECK(p_light) << "Got a Light which is not a PointLight.";

    Vector3 point_to_light =
        data.position.VectorTo(p_light->position()).Normalized();
    Vector3 point_to_camera =
        data.position.VectorTo(scene.camera().position()).Normalized();
    Vector3 normal = data.normal.Normalized();
    Scalar prod = point_to_light.Dot(normal);

    // Flip normal if the light is inside the element.
    if (prod < 0) {
      normal = -normal;
    }

    // Add diffuse contribution.
    Color3 diff = material.diffuse() * p_light->color();
    diffuse += (diff * prod).Clamped();

    // Add specular contribution.
    Color3 spec = material.specular() * p_light->color();
    Vector3 reflection = (-point_to_light).ReflectedOnPlane(normal);

    // Flip reflection if the camera is not on the same side of the element as
    // the light.
    if (prod < 0) {
      reflection = -reflection;
    }

    // Prevent cosine from turning positive through exponentiation.
    Scalar cosine = reflection.Dot(point_to_camera);
    cosine = cosine < 0 ? 0 : cosine;
    cosine = cosine > 1 ? 1 : cosine;

    specular += (spec * pow(cosine, material.shininess())).Clamped();
  }

  return (emission + ambient + diffuse + specular).Clamped();
}
