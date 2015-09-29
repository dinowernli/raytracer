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
 * Author: Dino Wernli
 */

#include "scene/geometry/triangle.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Triangle::Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
                   const Material& material, const Vector3* n1,
                   const Vector3* n2, const Vector3* n3)
    : Element(material, &(new BoundingBox(c1))->Include(c2).Include(c3)) {
  Vector3 normal = c1.VectorTo(c2).Cross(c1.VectorTo(c3)).Normalized();
  vertex1_.reset(new Vertex(c1, n1 == NULL ? normal : n1->Normalized()));
  vertex2_.reset(new Vertex(c2, n2 == NULL ? normal : n2->Normalized()));
  vertex3_.reset(new Vertex(c3, n3 == NULL ? normal : n3->Normalized()));
}

Triangle::Triangle(const Point3* c1, const Point3* c2, const Point3* c3,
                   const Vector3* n1, const Vector3* n2, const Vector3* n3,
                   const Material& material)
    : Element(material, &(new BoundingBox(*c1))->Include(*c2).Include(*c3)),
      vertex1_(new Vertex(c1, n1)), vertex2_(new Vertex(c2, n2)),
      vertex3_(new Vertex(c3, n3)) {
}

Triangle::~Triangle() {
}

bool Triangle::Intersect(const Ray& ray, IntersectionData* data) const {
  Vector3 edge12(vertex1_->point().VectorTo(vertex2_->point()));
  Vector3 edge13(vertex1_->point().VectorTo(vertex3_->point()));

  Vector3 dir_cross_first(ray.direction().Cross(edge13));
  Scalar determinant = edge12.Dot(dir_cross_first);
  if (determinant > -EPSILON && determinant < EPSILON) {
    return false;
  }
  Scalar invdet = 1 / determinant;

  // Compute barycentric u.
  Vector3 vertex_to_origin(vertex1_->point().VectorTo(ray.origin()));
  Scalar u = vertex_to_origin.Dot(dir_cross_first) * invdet;
  if (u < 0 || u > 1) {
    return false;
  }

  // Compute barycentric v.
  Vector3 plane_normal = vertex_to_origin.Cross(edge12);
  Scalar v = ray.direction().Dot(plane_normal) * invdet;
  if (v < 0 || v + u > 1) {
    return false;
  }

  Scalar t = edge13.Dot(plane_normal) * invdet;
  bool found = ray.InRange(t) && (data == NULL || t < data->t);
  if (found && data != NULL) {
    data->set_element(this);
    data->position = ray.PointAt(t);
    data->normal = vertex1_->normal() * (1 - u - v) + vertex2_->normal() * u
                   + vertex3_->normal() * v;
    data->material = &material();
    data->t = t;
  }
  return found;
}
