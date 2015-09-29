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
 * Unit tests for the Ray class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "util/ray.h"

namespace {

TEST(Ray, DirectionNormalized) {
  Vector3 direction(3, 4, 5);
  Ray ray(Point3(0, 0, 0), direction);
  EXPECT_DOUBLE_EQ(1, ray.direction().Length());
}

TEST(Ray, PointAt) {
  Point3 origin(1, 2, 3);
  Vector3 direction(0, -3, 1);
  Ray ray(origin, direction);

  Scalar t = 17;
  Point3 location = ray.PointAt(t);
  Point3 expected = origin + t * direction.Normalized();

  EXPECT_DOUBLE_EQ(expected.x(), location.x());
  EXPECT_DOUBLE_EQ(expected.y(), location.y());
  EXPECT_DOUBLE_EQ(expected.z(), location.z());
}

TEST(Ray, Range) {
  Ray ray(Point3(0, 0, 0), Vector3(0, 1, 0), 4, 6);

  EXPECT_TRUE(ray.InRange(4));
  EXPECT_TRUE(ray.InRange(4.1));
  EXPECT_TRUE(ray.InRange(5));
  EXPECT_TRUE(ray.InRange(6));

  EXPECT_FALSE(ray.InRange(3.9));
  EXPECT_FALSE(ray.InRange(-17));
  EXPECT_FALSE(ray.InRange(17));
}

}  // namespace
