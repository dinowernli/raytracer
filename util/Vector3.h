/*
 * A 3-dimensional vector. Defined entirely in this header for performance
 * reasons. Differs from Point3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>

#include "util/Numeric.h"
#include "util/Point3.h"

class Vector3 {
 public:
  Vector3() : x_(0), y_(0), z_(0) {}
  Vector3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}
  Vector3(const Vector3& other) : x_(other.x_), y_(other.y_), z_(other.z_) {}

  const Scalar& x() { return x_; }
  const Scalar& y() { return y_; }
  const Scalar& z() { return z_; }
  const Scalar& x() const { return x_; }
  const Scalar& y() const { return y_; }
  const Scalar& z() const { return z_; }

  Scalar SquaredLength() {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  Scalar Length() {
    return sqrt(SquaredLength());
  }

  // Returns a new vector representing the normalized version of *this.
  Vector3 Normalized() {
    return Vector3(*this) /= Length();
  }

  Vector3& operator/=(const Scalar& rhs) {
    x_ /= rhs;
    y_ /= rhs;
    z_ /= rhs;
    return *this;
  }

 private:
  Scalar x_, y_, z_;
};

// Addition operators.
inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Point3 operator+(const Point3& lhs, const Vector3& rhs)
{
  return Point3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

// Subtraction operators.
inline Vector3 operator-(const Point3& lhs, const Vector3& rhs) throw()
{
  return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) throw()
{
  return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

// Multiplication operators.
inline Vector3 operator*(const Scalar& lhs, const Vector3& rhs)
{
  return Vector3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

inline Vector3 operator*(const Vector3& lhs, const Scalar& rhs) throw()
{
  return Vector3(rhs * lhs.x(), rhs * lhs.y(), rhs * lhs.z());
}

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

#endif  /* VECTOR3_H_ */
