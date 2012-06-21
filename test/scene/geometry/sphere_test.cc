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
