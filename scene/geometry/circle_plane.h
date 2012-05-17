/*
 * A plane which alternates material circularly around its point.
 * Author: Dino Wernli
 */

#ifndef CIRCLE_PLANE_H_
#define CIRCLE_PLANE_H_

#include "scene/geometry/plane.h"

class CirclePlane : public Plane {
 public:
  CirclePlane(const Point3& point, const Vector3& normal, Scalar radius,
              const Material* ring_material, const Material* main_material);
  virtual ~CirclePlane();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

 private:
  Scalar radius_;
  const Material* ring_material_;
};

#endif  /* CIRCLE_PLANE_H_ */
