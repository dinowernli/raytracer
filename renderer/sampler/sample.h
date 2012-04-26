/*
 * A class which represents a sample of an image.
 * Author: Dino Wernli
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "util/color3.h"
#include "util/no_copy_assign.h"

class Sample {
 public:
  // Creates a sample of size 1. The color defaults to black.
  Sample(size_t x, size_t y) : x_(x), y_(y), size_x_(1), size_y_(1) { }
  Sample(size_t x, size_t y, size_t size_x, size_t size_y)
      : x_(x), y_(y), size_x_(size_x), size_y_(size_y) { }
  ~Sample() { }
  NO_COPY_ASSIGN(Sample);

  const Color3& color() const { return color_; }
  const size_t& x() const { return x_; }
  const size_t& y() const { return y_; }

  void set_color(const Color3& color) { color_ = color; }

 private:
  size_t x_;
  size_t y_;

  size_t size_x_;
  size_t size_y_;

  Color3 color_;
};

#endif  /* SAMPLE_H_ */
