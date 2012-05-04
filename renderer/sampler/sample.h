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

  // Creates a sample of size 1 at (0, 0). The color defaults to black.
  Sample() : x_(0), y_(0), size_x_(1), size_y_(1) { };
  ~Sample() { }

  // TODO(dinow): Somehow, WorkerMain() in renderer.cc seems to indicate that
  // std::vector requires this class to have a copy constructor. Possibly
  // uncomment after further investigation.
  // NO_COPY_ASSIGN(Sample);

  const Color3& color() const { return color_; }
  const size_t& x() const { return x_; }
  const size_t& y() const { return y_; }

  void set_color(const Color3& color) { color_ = color; }
  void set_x(size_t x) { x_ = x; }
  void set_y(size_t y) { y_ = y; }

 private:
  size_t x_;
  size_t y_;

  size_t size_x_;
  size_t size_y_;

  Color3 color_;
};

#endif  /* SAMPLE_H_ */
