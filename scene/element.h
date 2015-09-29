// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
class Material;
class Ray;

class Element {
 public:
  virtual ~Element() {}

  // Returns whether the ray intersects this element. If this returns true and
  // data != NULL, information about the first intersection is stored in data.
  virtual bool Intersect(const Ray& ray,
                         IntersectionData* data = NULL) const = 0;

  // Returns NULL if the object has no bounding box.
  const BoundingBox* bounding_box() const { return bounding_box_.get(); }

  const bool IsBounded() const { return bounding_box() != NULL; }

  const Material& material() const { return material_; }

 protected:
  // Only to be called by subclasses who wish to initialize the bounding box.
  // Takes ownership of the passed BoundingBox.
  Element(const Material& material, BoundingBox* box = NULL)
      : bounding_box_(box), material_(material) {}

 private:
  std::unique_ptr<BoundingBox> bounding_box_;

  const Material& material_;
};

#endif  /* ELEMENT_H_ */
