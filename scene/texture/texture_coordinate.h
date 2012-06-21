/*
 * Author: Dino Wernli
 */

#ifndef TEXTURE_COORDINATE_H_
#define TEXTURE_COORDINATE_H_

#include "util/numeric.h"

struct TextureCoordinate {
  // After initialization, Valid() will be false.
  TextureCoordinate() { Invalidate(); }
  bool Valid() const { return s >= 0 && t >= 0; }
  void Set(Scalar ss, Scalar tt) { s = ss; t = tt; }
  void Invalidate() { Set(-1, -1); }

  Scalar s, t;
};

#endif  /* TEXTURE_COORDINATE_H_ */
