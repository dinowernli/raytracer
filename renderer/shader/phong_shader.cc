/*
 * Author: Dino Wernli
 */

#include "phong_shader.h"

#include <cassert>
#include <cmath>

#include "renderer/intersection_data.h"
#include "scene/material.h"
#include "scene/point_light.h"
#include "scene/scene.h"

PhongShader::PhongShader() {
}

PhongShader::~PhongShader() {
}

Color3 PhongShader::Shade(const IntersectionData& data, const Scene& scene) {
  if (data.element == NULL) {
    return scene.background();
  }

  const Material& material = *data.material;
  const Color3& background = scene.background();

  Color3 emission(material.emission().Clamped());
  Color3 ambient((material.ambient() * background).Clamped());
  Color3 diffuse(0, 0, 0);
  Color3 specular(0, 0, 0);

  std::vector<const Light*> lights;
  scene.GetNonOccludedLights(data.position, &lights);

  for (auto it = lights.begin(); it != lights.end(); ++it) {
    // TODO(dinow): Hack since we only have point lights anyway.
    const PointLight* light = dynamic_cast<const PointLight *>(*it);
    assert(light);

    Vector3 point_to_light =
        data.position.VectorTo(light->position()).Normalized();
    Vector3 point_to_camera =
        data.position.VectorTo(scene.camera().position()).Normalized();
    Vector3 normal = data.normal.Normalized();
    Scalar prod = point_to_light.Dot(normal);

    // Flip normal if the light is inside the element.
    if (prod < 0) {
      normal = -normal;
    }

    // Add diffuse contribution.
    Color3 diff = material.diffuse() * light->color();
    diffuse += (diff * prod).Clamped();

    // Add specular contribution.
    Color3 spec = material.specular() * light->color();
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
