/*
 * An axis-aligned bounding box in 3D space.
 * Author: Dino Wernli
 */

#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "util/numeric.h"

class Point3;

class BoundingBox {
 public:
  BoundingBox(const Point3& point);
  virtual ~BoundingBox();

  // Changes the bounding box to include point.
  BoundingBox& Include(const Point3& point);

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
