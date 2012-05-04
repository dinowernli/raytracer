/*
 * A 3-dimensional vector. Defined entirely in this header for performance
 * reasons. Differs from Point3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>
#include <cstddef>

#include "util/axis.h"
#include "util/numeric.h"

class Point3;
class Vector3;

Vector3 operator*(const Scalar& lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& lhs, const Vector3& rhs);

class Vector3 {
 public:
  Vector3() : x_(0), y_(0), z_(0) {}
  Vector3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}

  const Scalar& x() const { return x_; }
  const Scalar& y() const { return y_; }
  const Scalar& z() const { return z_; }

  const Scalar& operator[](Axis axis) const {
    if (axis == Axis::x()) {
      return x_;
    } else if (axis == Axis::y()) {
      return y_;
    } else {
      return z_;
    }
  }

  Scalar& operator[](Axis axis) {
    if (axis == Axis::x()) {
      return x_;
    } else if (axis == Axis::y()) {
      return y_;
    } else {
      return z_;
    }
  }

  Scalar SquaredLength() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  Scalar Length() const {
    return sqrt(SquaredLength());
  }

  // Returns a new vector representing the normalized version of *this.
  Vector3 Normalized() const {
    return Vector3(*this) /= Length();
  }

  // Normalize the vector in place.
  void Normalize() {
    (*this) /= Length();
  }

  // Returns the cross product of *this times other.
  Vector3 Cross(const Vector3& other) const {
    return Vector3(
      y_ * other.z_ - z_ * other.y_,
      z_ * other.x_ - x_ * other.z_,
      x_ * other.y_ - y_ * other.x_
    );
  }

  Scalar Dot(const Vector3& other) const {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
  }

  // Returns this vector reflected on the plane given by "normal".
  Vector3 ReflectedOnPlane(const Vector3& normal) const {
    return ((*this) - 2 * normal.Dot(*this) * normal).Normalized();
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

// Unary operators.
inline Vector3 operator-(const Vector3& other) {
  return Vector3(-other.x(), -other.y(), -other.z());
}

// Addition operators.
inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

// Multiplication operators.
inline Vector3 operator*(const Scalar& lhs, const Vector3& rhs)
{
  return Vector3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

inline Vector3 operator*(const Vector3& lhs, const Scalar& rhs)
{
  return Vector3(rhs * lhs.x(), rhs * lhs.y(), rhs * lhs.z());
}

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
  return Vector3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

template<class OStream>
OStream& operator<<(OStream& os, const Vector3& v) {
  return os << "(v: " << v.x() << ", " << v.y() << ", " << v.z() << ")";
}

#endif  /* VECTOR3_H_ */
