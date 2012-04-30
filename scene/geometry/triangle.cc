/*
 * Author: Dino Wernli
 */

#include "scene/geometry/triangle.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

// TODO(dinow): Use array initializer as soon as all compilers can handler it.
Triangle::Triangle(const Point3& c1, const Point3& c2, const Point3& c3) {
    //: corners_({c1, c2, c3}) {
  corners_[0] = c1;
  corners_[1] = c2;
  corners_[2] = c3;
}

Triangle::~Triangle() {
}

bool Triangle::Intersect(const Ray& ray, IntersectionData* data) {
  // TODO(dinow): Actually test for intersection.s
  return false;
}

