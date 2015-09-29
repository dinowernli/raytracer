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
 * Contains the information about an intersection.
 * Author: Dino Wernli
 */

#ifndef INTERSECTIONDATA_H_
#define INTERSECTIONDATA_H_

#include "scene/texture/texture_coordinate.h"
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

  // Returns whether or not the intersection has valid 2D texture coordinates.
  // Note that 3D texture coordinates are just given by position.
  bool HasTextureCoordinates() const { return texture_coordinate.Valid(); }

  // Returns the intersected element or NULL if it is not an element.
  const Element* element() const { return element_; }

  // Returns the intersected light or NULL if it is not a light.
  const Light* light() const { return light_; }

  Scalar t;
  Point3 position;
  Vector3 normal;
  const Material* material;
  TextureCoordinate texture_coordinate;

 private:
  const Element* element_;
  const Light* light_;
};

#endif /* INTERSECTIONDATA_H_ */
