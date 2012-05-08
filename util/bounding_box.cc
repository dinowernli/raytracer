/*
 * Author: Dino Wernli
 */

#include "bounding_box.h"

#include <algorithm>
#include <glog/logging.h>

#include "util/numeric.h"
#include "util/ray.h"


BoundingBox::BoundingBox() {
  Init();
}

BoundingBox::BoundingBox(const Point3& p) {
  Init();
  Include(p);
}

BoundingBox::BoundingBox(const Point3& p1, const Point3& p2) {
  Init();
  Include(p1);
  Include(p2);
}

BoundingBox::~BoundingBox() {
}

void BoundingBox::Init() {
  Scalar inf = std::numeric_limits<Scalar>::infinity();
  xmin_ = inf; xmax_ = -inf;
  ymin_ = inf; ymax_ = -inf;
  zmin_ = inf; zmax_ = -inf;
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
  Include(other.min());
  Include(other.max());
  return *this;
}

bool BoundingBox::Intersect(const Ray& ray, Scalar* t_near,
                            Scalar* t_far) const {
  *t_near = -std::numeric_limits<Scalar>::infinity();
  *t_far = std::numeric_limits<Scalar>::infinity();

  Axis axes[3] = { Axis::x(), Axis::y(), Axis::z() };
  for(size_t i = 0; i < 3; ++i) {
    if (!AxisIntersect(axes[i], ray, t_near, t_far)) {
      return false;
    }
  }

  // Only return true if the [t_near, t_far] overlaps with [ray_min, ray_max].
  return (*t_far >= ray.min_t()) || (*t_near <= ray.max_t());
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

    if (*t_near > *t_far) {
      return false;
    }
  }
  return true;
}
