/*
 * An RGB color. Values are stored between 0 and 1.
 * Author: Dino Wernli
 */

#ifndef COLOR3_H_
#define COLOR3_H_

#include "util/Numeric.h"

class Color3 {
 public:
  Color3() : r_(0), g_(0), b_(0) {}
  Color3(const Intensity& r, const Intensity& g, const Intensity& b)
      : r_(r), g_(g), b_(b) {}

  const Intensity& r() const { return r_; }
  const Intensity& g() const { return g_; }
  const Intensity& b() const { return b_; }

 private:
  Intensity r_, g_, b_;
};

#endif  /* COLOR3_H_ */
