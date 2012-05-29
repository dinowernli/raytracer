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
