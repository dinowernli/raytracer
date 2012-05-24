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
