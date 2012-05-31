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
