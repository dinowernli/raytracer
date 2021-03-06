// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/*
 * A 3-dimensional point. Defined entirely in this header for performance
 * reasons. Differs from Vector3 in which operations are supported.
 * Author: Dino Wernli
 */

#ifndef POINT3_H_
#define POINT3_H_

#include <cmath>

#include "proto/util/point_data.pb.h"
#include "util/axis.h"
#include "util/numeric.h"
#include "util/vector3.h"

using raytracer::PointData;

class Point3 {
 public:
  Point3() : x_(0), y_(0), z_(0) {}

  Point3(const Scalar& x, const Scalar& y, const Scalar& z)
      : x_(x), y_(y), z_(z) {}

  // Returns a vector from *this to target.
  Vector3 VectorTo(const Point3& target) const {
    return Vector3(target.x_ - x_, target.y_ - y_, target.z_ - z_);
  }

  // Returns a vector from the origin to this point.
  Vector3 VectorFromOrigin() const {
    return Vector3(x_, y_, z_);
  }

  // Replaces the data of this point with the data from other.
  void ReplaceWith(const Point3& other) {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
  }

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

  static Scalar SquaredDistance(const Point3& first, const Point3& second) {
    return first.VectorTo(second).SquaredLength();
  }

  static Scalar Distance(const Point3& first, const Point3& second) {
    return sqrt(SquaredDistance(first, second));
  }

 private:
  Scalar x_, y_, z_;
};

inline Point3 operator*(const Scalar& lhs, const Point3& rhs)
{
  return Point3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}

inline Point3 operator+(const Point3& lhs, const Vector3& rhs)
{
  return Point3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Point3 operator-(const Point3& lhs, const Vector3& rhs)
{
  return Point3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

template<class OStream>
OStream& operator<<(OStream& os, const Point3& p) {
  return os << "(p: " << p.x() << ", " << p.y() << ", " << p.z() << ")";
}

#endif  /* POINT3_H_ */
