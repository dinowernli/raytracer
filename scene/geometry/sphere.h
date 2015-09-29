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

  mutable Random random_;
};

#endif  /* SPHERE_H_ */
