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
 * A mesh of triangles. Stores all vertices of the mesh and knows how to produce
 * light-weight elements.
 * Author: Dino Wernli
 */

#ifndef MESH_H_
#define MESH_H_

#include<memory>
#include<vector>

#include "util/numeric.h"

class Element;
class Material;
class Point3;
class Vector3;

class Mesh {
 public:
  // Does not take ownership of the passed material.
  Mesh();
  virtual ~Mesh();

  // Adds a copy of the passed point and returns the index of the new point.
  // The mesh guarantees that every new point increases the index by 1.
  size_t AddPoint(const Point3& point);

  // Adds a copy of the passed normal and returns the index of the new normal.
  // The mesh guarantees that every new normal increases the index by 1.
  size_t AddNormal(const Vector3& normal);

  // Declares a triangle using the three passed vertex indices.
  void AddTriangle(size_t v1, size_t n1, size_t v2, size_t n2, size_t v3,
                   size_t n3);

  // Adds light-weight triangles to target. The caller takes ownership of the
  // produced triangles.
  void CreateElements(std::vector<std::unique_ptr<Element>>* target) const;

  // Mimics the old transformation from the course XML files.
  // WARNING: Is not intuitive to use, but needed for compatibility.
  void Transform(Scalar scale, const Vector3& translation);

  // Recomputes the vertex normals from the triangles by averaging the normals
  // of the faces of the surrounding triangles.
  void InferNormals();

  // Does not take ownership of the passed material.
  void set_material(const Material* material) { material_ = material; }

 private:
  struct TriangleDescriptor {
    TriangleDescriptor(size_t p1_, size_t n1_, size_t p2_, size_t n2_,
                       size_t p3_, size_t n3_)
        : p1(p1_), n1(n1_), p2(p2_), n2(n2_), p3(p3_), n3(n3_) {
    }
    size_t p1, n1, p2, n2, p3, n3;
  };

  std::vector<Point3> points_;
  std::vector<Vector3> normals_;
  std::vector<TriangleDescriptor> descriptors_;
  const Material* material_;
};

#endif  /* MESH_H_ */
