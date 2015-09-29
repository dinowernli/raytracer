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
 * A utility class which provides uniformly distributed values.
 * Author: Dino Wernli
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

#include "util/numeric.h"

class Random {
 public:
  Random() {
    std::random_device device;
    random_engine_.seed(device());
    distribution_ = std::uniform_real_distribution<Scalar>(0, 1);
  }

  // Returns a uniform random number in [-boundary, boundary].
  Scalar Get(Scalar boundary) {
    return Get(-boundary, boundary);
  }

  // Returns a uniform random number in [lower, upper].
  Scalar Get(Scalar lower, Scalar upper) {
    return distribution_(random_engine_) * (upper - lower) + lower;
  }

 private:
  std::mt19937 random_engine_;
  std::uniform_real_distribution<Scalar> distribution_;
};

#endif  /* RANDOM_H_ */
