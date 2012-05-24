/*
 * Unit tests for the Ray class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "util/ray.h"

static bool IsReasonablyClose(Scalar expected, Scalar actual) {
  return (actual > expected - EPSILON) && (actual < expected + EPSILON);
}

TEST(Ray, DirectionNormalized) {
  Vector3 direction(3, 4, 5);
  Ray ray(Point3(0, 0, 0), direction);
  Scalar length = ray.direction().Length();
  EXPECT_TRUE(IsReasonablyClose(1, length)) << "Lenght: " << length;
}
