/*
 * A special type of texture which maps a color to each pair of 2D texture
 * coordinates.
 * Author: Dino Wernli
 */

#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_

#include <glog/logging.h>

#include "renderer/intersection_data.h"
#include "scene/texture/texture.h"

class Texture2D : public Texture {
 public:
  virtual ~Texture2D() {}
  virtual Color3 Evaluate(const IntersectionData& data) const {
    CHECK(data.HasTextureCoordinates()) << "Attempted to evaluate 2D texture "
                                        << "without 2D texture coordinates";
    return Evaluate2D(data);
  }

 protected:
  virtual Color3 Evaluate2D(const IntersectionData& data) const = 0;
};

#endif  /* TEXTURE_2D_H_ */
