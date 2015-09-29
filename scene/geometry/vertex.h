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
 * Consists of a point and a normal in 3D space. Used as a building block of
 * triangles.
 *
 * Author: Dino Wernli
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "util/point3.h"
#include "util/vector3.h"

class Vertex {
 public:
  // Copies the passed point and normal and allocated copies.
  Vertex(const Point3& point, const Vector3& normal)
      : point_(new Point3(point)), normal_(new Vector3(normal)),
        owns_data_(true) {
  }

  // Takes no ownership of the passed pointers and copies them. The underlying
  // data is referenced (if it changes, so will this vertex).
  Vertex(const Point3* point, const Vector3* normal)
      : point_(point), normal_(normal), owns_data_(false) {
  }

  ~Vertex() {
    if (owns_data_) {
      delete point_;
      delete normal_;
    }
  }

  const Point3& point() const { return *point_; }
  const Vector3& normal() const { return *normal_; }

 private:
  const Point3* point_;
  const Vector3* normal_;

  // This flag is false iff the data was allocated in a mesh and the vertex is
  // not expected to clean it up.
  bool owns_data_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Vertex& v)
{
  return os << "(vertex: " << v.point() << ", " << v.normal() << ")";
}

#endif  /* VERTEX_H_ */
