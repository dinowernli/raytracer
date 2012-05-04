/*
 * Author: Dino Wernli
 */

#include "bounding_box.h"

#include <algorithm>
#include <glog/logging.h>

#include "util/ray.h"

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
  DVLOG(2) << "Bounding box include: ";
  DVLOG(2) << "\tThis: " << *this;
  DVLOG(2) << "\tOther: " << other;
  Include(other.min());
  Include(other.max());
  DVLOG(2) << "\tFinal: " << *this;
  return *this;
}

bool BoundingBox::Intersect(const Ray& ray, Scalar* t_near,
                            Scalar* t_far) const {
  *t_near = -std::numeric_limits<Scalar>::infinity();
  *t_far = std::numeric_limits<Scalar>::infinity();

  bool intersected = AxisIntersect(Axis::x(), ray, t_near, t_far)
        && AxisIntersect(Axis::y(), ray, t_near, t_far)
        && AxisIntersect(Axis::z(), ray, t_near, t_far);
        && (!(*t_far < ray.min_t()))  && (!(*t_near > ray.max_t()));

  DVLOG(2) << "t_near: " << *t_near << ", t_far: " << *t_far
           << ", intersected: " << intersected;

  return intersected;
}

bool BoundingBox::AxisIntersect(Axis axis, const Ray& ray,
                                Scalar* t_near, Scalar* t_far) const {
  Scalar box_min = min()[axis];
  Scalar box_max = max()[axis];
  Scalar origin = ray.origin() [axis];
  Scalar direction = ray.direction() [axis];

  if (direction == 0) {
    if (origin < box_min || origin > box_max) {
      return false;
    }
  } else {
    Scalar t1 = (box_min - origin) / direction;
    Scalar t2 = (box_max - origin) / direction;

    using std::swap;
    if (t1 > t2) swap(t1, t2);

    if (t1 > *t_near) *t_near = t1;
    if (t2 < *t_far) *t_far = t2;

    if (t_near > t_far) {
      DVLOG(2) << "Missed box entirely";
      return false;
    }
  }
  return true;
}
