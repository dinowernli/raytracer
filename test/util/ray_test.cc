/*
 * Unit tests for the Ray class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "util/ray.h"

TEST(Ray, DirectionNormalized) {
  Vector3 direction(3, 4, 5);
  Ray ray(Point3(0, 0, 0), direction);
  Scalar length = ray.direction().Length();
  EXPECT_DOUBLE_EQ(1, length) << "Length: " << length;
}
