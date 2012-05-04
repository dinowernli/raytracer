/*
 * An axis-aligned bounding box in 3D space.
 * Author: Dino Wernli
 */

#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "util/numeric.h"
#include "util/point3.h"

class BoundingBox {
 public:
  // Creates a box containing exactly the passed point.
  BoundingBox(const Point3& point);

  // Creates a minimal box which contains both points.
  BoundingBox(const Point3& p1, const Point3& p2);

  virtual ~BoundingBox();

  // Changes the bounding box to include point.
  BoundingBox& Include(const Point3& point);

  // Changes the bounding box to include the other bounding box.
  BoundingBox& Include(const BoundingBox& other);

  Point3 min() const { return Point3(xmin_, ymin_, zmin_); }
  Point3 max() const { return Point3(xmax_, ymax_, zmax_); }

 private:
  Scalar xmin_, xmax_, ymin_, ymax_, zmin_, zmax_;
};

template<class OStream>
OStream& operator<<(OStream& os, const BoundingBox& box) {
  return os << "(box: "
    "(" << box.xmin_ << ' ' << box.xmax_ << ") "
    "(" << box.ymin_ << ' ' << box.ymax_ << ") "
    "(" << box.zmin_ << ' ' << box.zmax_ << "))";
}

#endif  /* BOUNDING_BOX_H_ */
