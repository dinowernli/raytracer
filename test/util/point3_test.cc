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
 * Unit tests for the Point3 class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "util/point3.h"

namespace {

TEST(Point3, Distance) {
  Point3 p(1, 2, 3);
  EXPECT_DOUBLE_EQ(0, Point3::Distance(p, p));

  Point3 q(1, 2, 4);
  EXPECT_DOUBLE_EQ(1, Point3::Distance(p, q));

  Point3 r(1, 2, 1);
  EXPECT_DOUBLE_EQ(2, Point3::Distance(p, r));
}

TEST(Point3, SquaredDistance) {
  Point3 p(1, 2, 3);
  EXPECT_DOUBLE_EQ(0, Point3::SquaredDistance(p, p));

  Point3 q(1, 2, 4);
  EXPECT_DOUBLE_EQ(1, Point3::SquaredDistance(p, q));

  Point3 r(1, 2, 1);
  EXPECT_DOUBLE_EQ(4, Point3::SquaredDistance(p, r));
}

}
