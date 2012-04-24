/*
 * A 3-dimensional vector. Defined entirely in this header for performance
 * reasons. Differs from Point3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "util/Numeric.h"

class Vector3 {
 public:
  Vector3() : x_(0), y_(0), z_(0) {}

  Vector3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}

  const Scalar& x() { return x_; }
  const Scalar& y() { return y_; }
  const Scalar& z() { return z_; }
  const Scalar& x() const { return x_; }
  const Scalar& y() const { return y_; }
  const Scalar& z() const { return z_; }

  Scalar SquaredLength() {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

 private:
  Scalar x_, y_, z_;
};

#endif  /* VECTOR3_H_ */
