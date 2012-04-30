/*
 * A shader which applies the phong shading method to determine the color.
 * Author: Dino Wernli
 */

#ifndef PHONG_SHADER_H_
#define PHONG_SHADER_H_

#include "renderer/shader/shader.h"
#include "util/no_copy_assign.h"

class PhongShader : public Shader {
 public:
  PhongShader();
  virtual ~PhongShader();
  NO_COPY_ASSIGN(PhongShader);

  virtual Color3 Shade(const IntersectionData& data, const Scene& scene);
};

#endif  /* PHONG_SHADER_H_ */
