/*
 * A class representing a single ray. It has an origin and a direction.
 * Author: Dino Wernli
 */

#ifndef RAY3_H_
#define RAY3_H_

#include "util/point3.h"
#include "util/vector3.h"

class Ray {
 public:
  Ray(const Point3& origin, const Vector3& direction)
      : origin_(origin), direction_(direction) {}

  const Vector3& direction() const { return direction_; }
  const Point3& origin() const { return origin_; }

 private:
  Point3 origin_;

  // The direction must always be a vector of length 1.
  Vector3 direction_;
};

#endif  /* RAY3_H_ */