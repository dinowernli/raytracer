/*
 * Author: Dino Wernli
 */

#include "scene/geometry/triangle.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Triangle::Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
                   const Vector3* n1, const Vector3* n2, const Vector3* n3) {
  Vector3 inferred_normal = c1.VectorTo(c2).Cross(c1.VectorTo(c3)).Normalized();
  vertex1_ = new Vertex(c1, n1 == NULL ? inferred_normal : *n1);
}

Triangle::~Triangle() {
  // TODO(dinow): As soon as meshes are implemented, add a test here to see if
  // the vertices really have to be deleted.
  delete vertex1_;
  delete vertex2_;
  delete vertex3_;
}

bool Triangle::Intersect(const Ray& ray, IntersectionData* data) {
  // TODO(dinow): Actually test for intersection.s
  return false;
}

