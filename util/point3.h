/*
 * A 3-dimensional point. Defined entirely in this header for performance
 * reasons. Differs from Vector3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef POINT3_H_
#define POINT3_H_

#include "util/numeric.h"
#include "util/vector3.h"

class Point3 {
 public:
  Point3() : x_(0), y_(0), z_(0) {}

  Point3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}

  // Returns a vector from *this to target.
  Vector3 VectorTo(const Point3& target) const {
    return Vector3(target.x_ - x_, target.y_ - y_, target.z_ - z_);
  }

  const Scalar& x() { return x_; }
  const Scalar& y() { return y_; }
  const Scalar& z() { return z_; }
  const Scalar& x() const { return x_; }
  const Scalar& y() const { return y_; }
  const Scalar& z() const { return z_; }

 private:
  Scalar x_, y_, z_;
};

inline Point3 operator+(const Point3& lhs, const Vector3& rhs)
{
  return Point3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Vector3 operator-(const Point3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

template<class OStream>
OStream& operator<<(OStream& os, const Point3& p)
{
  return os << "(p: " << p.x() << ", " << p.y() << ", " << p.z() << ")";
}

#endif  /* POINT3_H_ */
