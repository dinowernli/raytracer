/*
 * A sphere which can be added
 * Author: Dino Wernli
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "scene/element.h"
#include "util/point3.h"

class Material;

class Sphere : public Element {
 public:
  // Does not take ownership of material.
  Sphere(const Point3& center, Scalar radius, const Material& material);
  virtual ~Sphere();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

 private:
  Point3 center_;
  Scalar radius_;
};

#endif  /* SPHERE_H_ */
