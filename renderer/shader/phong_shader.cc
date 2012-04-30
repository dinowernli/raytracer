/*
 * Author: Dino Wernli
 */

#include "phong_shader.h"

#include "renderer/intersection_data.h"
#include "scene/material.h"
#include "scene/scene.h"

PhongShader::PhongShader() {
}

PhongShader::~PhongShader() {
}

Color3 PhongShader::Shade(const IntersectionData& data, const Scene& scene) {
  if (data.element == NULL) {
    return scene.background()->diffuse();
  }

  Color3 emission(0, 0, 0);
  Color3 ambient(0, 0, 0);
  Color3 diffuse(0, 0, 0);
  Color3 specular(0, 0, 0);

  // TODO(dinow): Do the actual phong shading.
  if (data.material != NULL) {
    return data.material->diffuse();
  } else {
    return Color3(1, 1, 1);
  }
}
