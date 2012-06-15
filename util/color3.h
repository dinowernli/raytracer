/*
 * An RGB color. Values are stored between 0 and 1.
 * Author: Dino Wernli
 */

#ifndef COLOR3_H_
#define COLOR3_H_

#include "proto/util/color_data.pb.h"
#include "util/numeric.h"

using raytracer::ColorData;

class Color3 {
 public:
  Color3() : r_(0), g_(0), b_(0) {}
  Color3(const Intensity& r, const Intensity& g, const Intensity& b)
      : r_(r), g_(g), b_(b) {}

  Color3 operator*(const Scalar& rhs) const {
    return Color3(r_ * rhs, g_ * rhs, b_ * rhs);
  }

  Color3 operator/(const Scalar& rhs) const {
    return Color3(r_ / rhs, g_ / rhs, b_ / rhs);
  }

  Color3& operator+=(const Color3& rhs) {
    r_ += rhs.r_;
    g_ += rhs.g_;
    b_ += rhs.b_;
    return *this;
  }

  // Returns a new color which contains the values of this clamped to [0, 1].
  Color3 Clamped() const {
    Scalar r = r_ < 0 ? 0 : r_;
    r = r > 1 ? 1 : r;

    Scalar g = g_ < 0 ? 0 : g_;
    g = g > 1 ? 1 : g;

    Scalar b = b_ < 0 ? 0 : b_;
    b = b > 1 ? 1 : b;

    return Color3(r, g, b);
  }

  const Intensity& r() const { return r_; }
  const Intensity& g() const { return g_; }
  const Intensity& b() const { return b_; }

 private:
  Intensity r_, g_, b_;
};

inline Color3 operator-(const Color3& lhs, const Color3& rhs)
{
  return Color3(lhs.r() - rhs.r(), lhs.g() - rhs.g(), lhs.b() - rhs.b());
}

inline Color3 operator+(const Color3& lhs, const Color3& rhs)
{
  return Color3(lhs.r() + rhs.r(), lhs.g() + rhs.g(), lhs.b() + rhs.b());
}

inline Color3 operator*(const Color3& lhs, const Color3& rhs)
{
  return Color3(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b());
}

inline Color3 operator*(const Scalar& lhs, const Color3& rhs)
{
  return Color3(lhs * rhs.r(), lhs * rhs.g(), lhs * rhs.b());
}

template<class OStream>
OStream& operator<<(OStream& os, const Color3& c) {
  return os << "(c: " << c.r() << ", " << c.g() << ", " << c.b() << ")";
}

#endif  /* COLOR3_H_ */
