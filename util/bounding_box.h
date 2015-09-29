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
 * An axis-aligned bounding box in 3D space.
 * Author: Dino Wernli
 */

#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "util/axis.h"
#include "util/numeric.h"
#include "util/point3.h"

class Ray;

class BoundingBox {
 public:
  // Creates an empty bounding box which contains nothing.
  BoundingBox();

  // Creates a box containing exactly the passed point.
  BoundingBox(const Point3& point);

  // Creates a minimal box which contains both points.
  BoundingBox(const Point3& p1, const Point3& p2);

  virtual ~BoundingBox();

  // Changes the bounding box to include point.
  BoundingBox& Include(const Point3& point);

  // Changes the bounding box to include the other bounding box.
  BoundingBox& Include(const BoundingBox& other);

  // Returns whether or not the ray intersects this bounding box. If it returns
  // true, the entry and exit points are stored in t_near and t_far. If there is
  // only one intersection point, it is stored in both.
  bool Intersect(const Ray& ray, Scalar* t_near, Scalar* t_far) const;

  Point3 min() const { return Point3(xmin_, ymin_, zmin_); }
  Point3 max() const { return Point3(xmax_, ymax_, zmax_); }

 private:
  // Initializer which allows code sharing between constructors.
  void Init();

  bool AxisIntersect(Axis axis, const Ray& ray, Scalar* t_min,
                     Scalar* t_max) const;

  Scalar xmin_, xmax_, ymin_, ymax_, zmin_, zmax_;
};

template<class OStream>
OStream& operator<<(OStream& os, const BoundingBox& box) {
  return os << "(box: " << box.min() << ", " << box.max() << ")";
}

#endif  /* BOUNDING_BOX_H_ */
