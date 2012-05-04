/*
 * Represents an element which can be rendered as part of a scene.
 * Author: Dino Wernli
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <cstddef>
#include <memory>

#include "util/bounding_box.h"

class IntersectionData;
class Ray;

class Element {
 public:
  virtual ~Element() {}

  // Returns whether the ray intersects this element. If this returns true and
  // data != NULL, information about the first intersection is stored in data.
  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) = 0;

  // Returns NULL if the object has no bounding box.
  const BoundingBox* bounding_box() const { return bounding_box_.get(); }

  const bool IsBounded() const { return bounding_box() != NULL; }

 protected:
  // Only to be called by subclasses who wish to initialize the bounding box.
  // Takes ownership of the passed BoundingBox.
  Element(BoundingBox* box = NULL) : bounding_box_(box) {}

 private:
  std::unique_ptr<BoundingBox> bounding_box_;
};

#endif  /* ELEMENT_H_ */
