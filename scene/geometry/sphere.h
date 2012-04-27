/*
 * A sphere which can be added
 * Author: Dino Wernli
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "scene/element.h"
#include "util/point3.h"

class Sphere : public Element {
 public:
  Sphere(const Point3& center, Scalar radius);
  virtual ~Sphere();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL);

 private:
  Point3 center_;
  Scalar radius_;
};

#endif  /* SPHERE_H_ */
