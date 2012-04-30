/*
 * Author: Dino Wernli
 */

#include "phong_shader.h"

#include "renderer/intersection_data.h"
#include "scene/material.h"

PhongShader::PhongShader() {
}

PhongShader::~PhongShader() {
}

Color3 PhongShader::Shade(const IntersectionData& data) {
  // TODO(dinow): Do the actual phong shading.
  if (data.material != NULL) {
    return data.material->diffuse();
  } else {
    return Color3(1, 1, 1);
  }
}
