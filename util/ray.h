/*
 * A class representing a single ray. It has an origin and a direction.
 * Author: Dino Wernli
 */

#ifndef RAY3_H_
#define RAY3_H_

#include <limits>

#include "util/point3.h"
#include "util/vector3.h"

class Ray {
 public:
  Ray(const Point3& origin, const Vector3& direction, Scalar min_t = 0,
      Scalar max_t = std::numeric_limits<Scalar>::max())
          : origin_(origin), direction_(direction), min_t_(min_t),
            max_t_(max_t) {
  }

  // Returns the unique point on the ray at distance t from the origin.
  Point3 PointAt(Scalar t) const {
    return origin_ + t * direction_;
  }

  // Returns whether or not t is within the range of this ray.
  bool InRange(Scalar t) const {
    return t >= min_t_ && t <= max_t_;
  }

  const Vector3& direction() const { return direction_; }
  const Point3& origin() const { return origin_; }
  Scalar min_t() const { return min_t_; }
  Scalar max_t() const { return max_t_; }

 private:
  Point3 origin_;

  // The direction must always be a vector of length 1.
  Vector3 direction_;

  // These values describe the range of the ray.
  Scalar min_t_;
  Scalar max_t_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Ray& r)
{
  return os << "(ray: " << r.origin() << ", " << r.direction() << ")";
}


#endif  /* RAY3_H_ */
