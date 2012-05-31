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

class IntersectionData {
 public:
  // Builds a new struct for this specific ray. The ray is necessary in order to
  // initialize "t" to the maximum "t" allowed by the ray.
  IntersectionData(const Ray& ray)
      : t(ray.max_t()), material(NULL), element_(NULL), light_(NULL) {}

  bool IntersectedElement() const { return element_ != NULL; }
  bool IntersectedLight() const { return light_ != NULL; }

  void set_element(const Element* element) {
    element_ = element;
    light_ = NULL;
  }

  void set_light(const Light* light) {
    element_ = NULL;
    light_ = light;
  }

  const Element* element() const { return element_; }
  const Light* light() const { return light_; }

  Scalar t;
  Point3 position;
  Vector3 normal;
  const Material* material;

 private:
  const Element* element_;
  const Light* light_;
};

#endif /* INTERSECTIONDATA_H_ */
