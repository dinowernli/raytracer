/*
 * A shader which applies the phong shading method to determine the color.
 * Author: Dino Wernli
 */

#ifndef PHONG_SHADER_H_
#define PHONG_SHADER_H_

#include "renderer/shader/shader.h"

class PhongShader : public Shader {
 public:
  PhongShader();
  virtual ~PhongShader();
  virtual Color3 Shade(const IntersectionData& data);
};

#endif  /* PHONG_SHADER_H_ */
