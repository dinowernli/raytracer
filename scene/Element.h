/*
 * Represents an element which can be rendered as part of a scene.
 * Author: Dino Wernli
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <cstddef>

class IntersectionData;
class Ray;

class Element {
 public:
  virtual ~Element() {}
  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) = 0;
};

#endif  /* ELEMENT_H_ */
