/*
 * Represents a plane which can be added to a scene.
 * Author: Dino Wernli
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "scene/element.h"
#include "util/point3.h"

class Material;

class Plane : public Element {
 public:
  Plane(const Point3& point, const Vector3& normal, const Material& material);
  virtual ~Plane();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  const Point3 point() const { return point_; }

 private:
  Point3 point_;

  // This should always have length 1.
  Vector3 normal_;
};

#endif  /* PLANE_H_ */
