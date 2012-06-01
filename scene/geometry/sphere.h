/*
 * A sphere which can be added
 * Author: Dino Wernli
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "scene/element.h"
#include "util/point3.h"
#include "util/random.h"

class Material;

class Sphere : public Element {
 public:
  // Does not take ownership of material.
  Sphere(const Point3& center, Scalar radius, const Material& material);
  virtual ~Sphere();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  // Returns a uniformly distributed random point on the surface of the sphere.
  // This guarantees that "point" is visible from the returned point.
  // TODO(dinow): This is not true, replace this my a method taking a normal.
  Point3 Sample(const Point3& point) const;

 private:
  Point3 center_;
  Scalar radius_;

  static Random random_;
};

#endif  /* SPHERE_H_ */
