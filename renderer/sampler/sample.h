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
 * A class which represents a sample of an image.
 * Author: Dino Wernli
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "util/color3.h"

class Sample {
 public:
  // Creates a sample of size 1. The color defaults to black.
  Sample(size_t x, size_t y) : x_(x), y_(y), offset_x_(0), offset_y_(0),
                               size_x_(1), size_y_(1) {}
  Sample(size_t x, size_t y, size_t size_x, size_t size_y)
      : x_(x), y_(y), offset_x_(0), offset_y_(0), size_x_(size_x),
        size_y_(size_y) {}

  // Creates a sample of size 1 at (0, 0). The color defaults to black.
  Sample() : x_(0), y_(0), offset_x_(0), offset_y_(0),
             size_x_(1), size_y_(1) {};
  ~Sample() {}

  const size_t& x() const { return x_; }
  const size_t& y() const { return y_; }
  const Scalar& offset_x() const { return offset_x_; }
  const Scalar& offset_y() const { return offset_y_; }
  const Color3& color() const { return color_; }
  const size_t& size_x() const { return size_x_; }
  const size_t& size_y() const { return size_y_; }

  void set_x(size_t x) { x_ = x; }
  void set_y(size_t y) { y_ = y; }
  void set_offset_x(Scalar offset_x) { offset_x_ = offset_x; }
  void set_offset_y(Scalar offset_y) { offset_y_ = offset_y; }
  void set_color(const Color3& color) { color_ = color; }
  void set_size_x(size_t size_x) { size_x_ = size_x; }
  void set_size_y(size_t size_y) { size_y_ = size_y; }

 private:
  // The sample (0, 0) represents the bottom left pixel.
  size_t x_;
  size_t y_;

  Scalar offset_x_;
  Scalar offset_y_;

  size_t size_x_;
  size_t size_y_;

  Color3 color_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Sample& s) {
  return os << "location: (" << s.x() << ", " << s.y() << ") size: ("
            << s.size_x() << ", " << s.size_y() << ") offset: (" << s.offset_x()
            << ", " << s.offset_y() << ")";
}

#endif  /* SAMPLE_H_ */
