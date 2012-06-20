/*
 * Maps an intersection to some texture color.
 * Author: Dino Wernli
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "util/color3.h"

class IntersectionData;

class Texture {
 public:
  virtual ~Texture() {}
  virtual Color3 Evaluate(const IntersectionData& data) const = 0;
};

#endif  /* TEXTURE_H_ */
