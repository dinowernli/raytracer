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
 * Unit tests for the random class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "util/random.h"

namespace {

TEST(Random, Interval) {
  Random random;
  Scalar r = random.Get(-0.3, 1.3);
  EXPECT_TRUE(r >= -0.3 && r <= 1.3);
  r = random.Get(-20, 17);
  EXPECT_TRUE(r >= -20 && r <= 17);
}

TEST(Random, Boundary) {
  Random random;
  Scalar r = random.Get(-0.3);
  EXPECT_TRUE(r >= -0.3 && r <= 0.3);
  r = random.Get(3);
  EXPECT_TRUE(r >= -3 && r <= 3);
}

}  // namespace
