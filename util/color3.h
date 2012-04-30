/*
 * An RGB color. Values are stored between 0 and 1.
 * Author: Dino Wernli
 */

#ifndef COLOR3_H_
#define COLOR3_H_

#include "util/numeric.h"

class Color3 {
 public:
  Color3() : r_(0), g_(0), b_(0) {}
  Color3(const Intensity& r, const Intensity& g, const Intensity& b)
      : r_(r), g_(g), b_(b) {}

  Color3 operator/(const Scalar& rhs) const {
    return Color3(r_ / rhs, g_ / rhs, b_ / rhs);
  }

  // Clamps all channels of this to the range [0, 1] in place.
  Color3& Clamp() {
    r_ = r_ < 0 ? 0 : r_;
    r_ = r_ > 1 ? 1 : r_;

    g_ = g_ < 0 ? 0 : g_;
    g_ = g_ > 1 ? 1 : g_;

    b_ = b_ < 0 ? 0 : b_;
    b_ = b_ > 1 ? 1 : b_;

    return *this;
  }

  const Intensity& r() const { return r_; }
  const Intensity& g() const { return g_; }
  const Intensity& b() const { return b_; }

 private:
  Intensity r_, g_, b_;
};

#endif  /* COLOR3_H_ */
