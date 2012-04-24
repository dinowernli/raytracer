/*
 * A class representing a single ray. It has an origin and a direction.
 * Author: Dino Wernli
 */

#ifndef RAY3_H_
#define RAY3_H_

#include "util/Point3.h"
#include "util/Vector3.h"

class Ray {
 public:
  Ray(const Point3& origin, const Vector3& direction)
      : origin_(origin), direction_(direction) {}

  const Vector3& direction() { return direction_; }
  const Point3& origin() { return origin_; }

 private:
  Point3 origin_;

  // The direction must always be a vector of length 1.
  Vector3 direction_;
};

#endif  /* RAY3_H_ */
