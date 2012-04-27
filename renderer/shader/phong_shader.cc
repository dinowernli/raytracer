/*
 * Author: Dino Wernli
 */

#include "phong_shader.h"

PhongShader::PhongShader() {
}

PhongShader::~PhongShader() {
}

Color3 PhongShader::Shade(const IntersectionData& data) {
  // TODO(dinow): Do the actual phong shading.
  return Color3(1, 1, 1);
}
