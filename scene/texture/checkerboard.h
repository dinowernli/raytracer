/*
 * A 2D texture which produces a checkerboard.
 * Author: Dino Wernli
 */

#ifndef CHECKERBOARD_H_
#define CHECKERBOARD_H_

#include "scene/texture/texture_2d.h"

class Checkerboard : public Texture2D {
 public:
  Checkerboard(Color3 first = Color3(0, 0, 0), Color3 second = Color3(1, 1, 1))
      : first_(first), second_(second) {}
  virtual ~Checkerboard() {}

 protected:
  virtual Color3 Evaluate2D(const IntersectionData& data) const {
    // TODO(dinow): Add parameter for checkerboard frequency.
    size_t ss = size_t(data.texture_coordinate.s * 10) & 1;
    size_t tt = size_t(data.texture_coordinate.t * 10) & 1;
    if (ss == tt) {
      return first_;
    } else {
      return second_;
    }
  }

 private:
  Color3 first_;
  Color3 second_;
};


#endif  /* CHECKERBOARD_H_ */
