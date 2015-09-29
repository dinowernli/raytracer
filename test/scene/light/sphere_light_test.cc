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
 * Unit tests for the sphere light.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "scene/light/sphere_light.h"

namespace {

TEST(SphereLight, GeneratedRayDoesNotIntersect) {
  Point3 center(1, -2, 6);
  Scalar radius = 5;
  Color3 color(1, 1, 1);

  SphereLight sphere_light(center, radius, color);

  for (int i = 0; i < 20; ++i) {
    Ray r = sphere_light.GenerateRay(Point3(100, 200, 4));
    IntersectionData data(r);
    EXPECT_FALSE(sphere_light.Intersect(r, &data)) << "Ray: " << r << ", t: "
                                                   << data.t;
  }
}

}


