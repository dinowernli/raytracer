/*
 * A 3-dimensional point. Defined entirely in this header for performance
 * reasons. Differs from Vector3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef POINT3_H_
#define POINT3_H_

#include "util/Numeric.h"

class Point3 {
 public:
  Point3() : x_(0), y_(0), z_(0) {}

  Point3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}

  const Scalar& x() { return x_; }
  const Scalar& y() { return y_; }
  const Scalar& z() { return z_; }
  const Scalar& x() const { return x_; }
  const Scalar& y() const { return y_; }
  const Scalar& z() const { return z_; }

 private:
  Scalar x_, y_, z_;
};

#endif  /* POINT3_H_ */
