/*
 * Contains the information about an intersection.
 * Author: Dino Wernli
 */

#ifndef INTERSECTIONDATA_H_
#define INTERSECTIONDATA_H_

#include "scene/element.h"
#include "scene/material.h"
#include "util/point3.h"
#include "util/ray.h"

struct IntersectionData {
  // Builds a new struct for this specific ray. The ray is necessary in order to
  // initialize "t" to the maximum "t" allowed by the ray.
  IntersectionData(const Ray& ray)
      : t(ray.max_t()), element(NULL), material(NULL) {}

  bool Intersected() const { return element != NULL; }

  Scalar t;
  Point3 position;
  Vector3 normal;
  const Element* element;
  const Material* material;
};

#endif /* INTERSECTIONDATA_H_ */
