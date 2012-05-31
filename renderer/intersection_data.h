/*
 * Contains the information about an intersection.
 * Author: Dino Wernli
 */

#ifndef INTERSECTIONDATA_H_
#define INTERSECTIONDATA_H_

#include "util/point3.h"
#include "util/ray.h"

class Element;
class Light;
class Material;

struct IntersectionData {
  // Builds a new struct for this specific ray. The ray is necessary in order to
  // initialize "t" to the maximum "t" allowed by the ray.
  IntersectionData(const Ray& ray)
      : t(ray.max_t()), element(NULL), material(NULL), light(NULL) {}

  bool IntersectedElement() const { return element != NULL; }
  bool IntersectedLight() const { return light != NULL; }

  Scalar t;
  Point3 position;
  Vector3 normal;
  const Element* element;
  const Material* material;
  const Light* light;
};

#endif /* INTERSECTIONDATA_H_ */
