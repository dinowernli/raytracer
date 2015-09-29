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
 * A triangle element which can be added to a scene.
 * Author: Dino Wernli
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "scene/element.h"
#include "scene/geometry/vertex.h"
#include "util/no_copy_assign.h"

class Material;

class Triangle : public Element {
 public:
  // Copies the points and normals. Does not take ownership of any passed data.
  // Any normal which is NULL is inferred as well as possible.
  Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
           const Material& material, const Vector3* n1 = NULL,
           const Vector3* n2 = NULL, const Vector3* n3 = NULL);

  // Takes no ownership of the passed data, all pointers are simply copied.
  Triangle(const Point3* c1, const Point3* c2, const Point3* c3,
           const Vector3* n1, const Vector3* n2, const Vector3* n3,
           const Material& material);

  virtual ~Triangle();
  NO_COPY_ASSIGN(Triangle);

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  const Vertex& vertex1() const { return *vertex1_; }
  const Vertex& vertex2() const { return *vertex2_; }
  const Vertex& vertex3() const { return *vertex3_; }

 private:
  std::unique_ptr<Vertex> vertex1_;
  std::unique_ptr<Vertex> vertex2_;
  std::unique_ptr<Vertex> vertex3_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Triangle& t)
{
  return os << "(triangle: " << t.vertex1() << ", " << t.vertex2() << ", "
            << t.vertex3() << ")";
}

#endif  /* TRIANGLE_H_ */
