/*
 * Represents an object which computes a color given the intersection data.
 * Author: Dino Wernli
 */

#ifndef SHADER_H_
#define SHADER_H_

#include "util/color3.h"

class IntersectionData;
class Scene;

class Shader {
 public:
  virtual ~Shader() { }
  virtual Color3 Shade(const IntersectionData& data, const Scene& scene) = 0;
};


#endif  /* SHADER_H_ */
