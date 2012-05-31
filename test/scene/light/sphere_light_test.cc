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


