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
 * Unit tests for the sphere element.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "scene/geometry/sphere.h"
#include "scene/material.h"

namespace {

TEST(Sphere, PointOnSurface) {
  Color3 c(0, 0, 0);
  Material dummy(NULL, NULL, NULL, NULL, 0, 0, 0, 0);

  Point3 target(1, 2, 3);
  Point3 center(17, -2, 8);
  Scalar radius = 3.5;
  Sphere sphere(center, radius, dummy);

  for (int i = 0; i < 20; ++i) {
    Point3 sampled = sphere.Sample(target);
    Scalar distance = Point3::Distance(center, sampled);
    EXPECT_DOUBLE_EQ(radius, distance);
    EXPECT_TRUE(center.VectorTo(target).Dot(sampled.VectorTo(target)) >= 0);
  }
}

}
