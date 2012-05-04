/*
 * Author: Dino Wernli
 */

#include "bounding_box.h"

#include <algorithm>

BoundingBox::BoundingBox(const Point3& p) : xmin_(p.x()), xmax_(p.x()),
    ymin_(p.y()), ymax_(p.y()), zmin_(p.z()), zmax_(p.z()) {
}

BoundingBox::BoundingBox(const Point3& p1, const Point3& p2)
    : xmin_(std::min(p1.x(), p2.x())), xmax_(std::max(p1.x(), p2.x())),
      ymin_(std::min(p1.y(), p2.y())), ymax_(std::max(p1.y(), p2.y())),
      zmin_(std::min(p1.z(), p2.z())), zmax_(std::max(p1.z(), p2.z())) {
}

BoundingBox::~BoundingBox() {
}

BoundingBox& BoundingBox::Include(const Point3& point) {
  using std::min;
  using std::max;
  xmin_ = min(xmin_, point.x());
  xmax_ = max(xmax_, point.x());
  ymin_ = min(ymin_, point.y());
  ymax_ = max(ymax_, point.y());
  zmin_ = min(zmin_, point.z());
  zmax_ = max(zmax_, point.z());
  return *this;
}

BoundingBox& BoundingBox::Include(const BoundingBox& other) {
  Include(Point3(other.xmin_, other.ymin_, other.zmin_));
  Include(Point3(other.xmax_, other.ymax_, other.zmax_));
  return *this;
}
