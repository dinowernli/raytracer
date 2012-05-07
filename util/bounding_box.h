/*
 * An axis-aligned bounding box in 3D space.
 * Author: Dino Wernli
 */

#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "util/axis.h"
#include "util/numeric.h"
#include "util/point3.h"

class Ray;

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

  // Returns whether or not the ray intersects this bounding box. If it returns
  // true, the entry and exit points are stored in t_near and t_far. If there is
  // only one intersection point, it is stored in both.
  bool Intersect(const Ray& ray, Scalar* t_near, Scalar* t_far) const;

  Point3 min() const { return Point3(xmin_, ymin_, zmin_); }
  Point3 max() const { return Point3(xmax_, ymax_, zmax_); }

 private:
  bool AxisIntersect(Axis axis, const Ray& ray, Scalar* t_min,
                     Scalar* t_max) const;

  Scalar xmin_, xmax_, ymin_, ymax_, zmin_, zmax_;
};

template<class OStream>
OStream& operator<<(OStream& os, const BoundingBox& box) {
  return os << "(box: " << box.min() << ", " << box.max() << ")";
}

#endif  /* BOUNDING_BOX_H_ */
