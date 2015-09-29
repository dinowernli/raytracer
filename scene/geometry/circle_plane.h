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
 * A plane which alternates material circularly around its point.
 * Author: Dino Wernli
 */

#ifndef CIRCLE_PLANE_H_
#define CIRCLE_PLANE_H_

#include "scene/geometry/plane.h"

class CirclePlane : public Plane {
 public:
  CirclePlane(const Point3& point, const Vector3& normal, Scalar radius,
              const Material& ring_material, const Material& main_material);
  virtual ~CirclePlane();

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

 private:
  Scalar radius_;
  const Material& ring_material_;
};

#endif  /* CIRCLE_PLANE_H_ */
