/*
 * Unit tests for the checkerboard.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "scene/texture/checkerboard.h"
#include "test/test_util.h"

namespace {

static IntersectionData DataWithCoords(Scalar ss, Scalar tt) {
  Ray r(Point3(0, 0, 0), Vector3(0, 0, 1));
  IntersectionData data(r);
  data.texture_coordinate.s = ss;
  data.texture_coordinate.t = tt;
  return data;
}

TEST(Checkerboard, IsChanging) {
  Color3 blue(0, 0, 1);
  Color3 red(1, 0, 0);
  Checkerboard cb(blue, red);

  EXPECT_TRUE(TestUtil::ColorsEqual(blue, cb.Evaluate(DataWithCoords(0, 0))));
  EXPECT_TRUE(TestUtil::ColorsEqual(red, cb.Evaluate(DataWithCoords(0, 0.1))));
}

}
