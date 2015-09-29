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
 * A type safe implementation of an axis in 3D space. Only the values 'x', 'y'
 * and 'z' are legal.
 * Author: Dino Wernli
 */

#ifndef AXIS_H_
#define AXIS_H_

#include <cstddef>

class Axis {
 public:
  // Build an Axis from the rest of "id" modulo 3.
  explicit Axis(size_t id) : id_(id % 3) {}

  static inline Axis x() { return Axis(0); }
  static inline Axis y() { return Axis(1); }
  static inline Axis z() { return Axis(2); }

  // Maps x to y, y to z and z to x.
  Axis Next() const { return Axis(id_ + 1); }
  bool operator==(const Axis& other) const { return other.id_ == id_; }

 private:
  size_t id_;
};

#endif  /* AXIS_H_ */
