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
  return Color3(0.7, 0.2, 0.4);
}
