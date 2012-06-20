/*
 * A texture which simply always returns a specific color.
 * Author: Dino Wernli
 */

#ifndef CONSTANT_TEXTURE_H_
#define CONSTANT_TEXTURE_H_

#include "scene/texture/texture.h"

class ConstantTexture : public Texture {
 public:
  ConstantTexture(const Color3& color) : color_(color) {}

  virtual Color3 Evaluate(const IntersectionData& data) const {
    return color_;
  }

 private:
  Color3 color_;
};

#endif  /* CONSTANT_TEXTURE_H_ */
