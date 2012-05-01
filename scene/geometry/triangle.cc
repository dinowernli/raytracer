/*
 * Author: Dino Wernli
 */

#include "scene/geometry/triangle.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Triangle::Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
                   const Material* material, const Vector3* n1,
                   const Vector3* n2, const Vector3* n3) : material_(material) {
  Vector3 inferred_normal = c1.VectorTo(c2).Cross(c1.VectorTo(c3)).Normalized();
  vertex1_ = new Vertex(c1, n1 == NULL ? inferred_normal : n1->Normalized());
  vertex2_ = new Vertex(c2, n2 == NULL ? inferred_normal : n2->Normalized());
  vertex3_ = new Vertex(c3, n3 == NULL ? inferred_normal : n3->Normalized());
}

Triangle::~Triangle() {
  // TODO(dinow): As soon as meshes are implemented, add a test here to see if
  // the vertices really have to be deleted.
  delete vertex1_;
  delete vertex2_;
  delete vertex3_;
}

bool Triangle::Intersect(const Ray& ray, IntersectionData* data) {
  Vector3 edge12(vertex1_->point().VectorTo(vertex2_->point()));
  Vector3 edge13(vertex1_->point().VectorTo(vertex3_->point()));

  Vector3 dir_cross_first(ray.direction().Cross(edge13));
  Scalar determinant = edge12.Dot(dir_cross_first);
  if (determinant > EPSILON && determinant < EPSILON) {
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
  if (v < 0 || v > 1) {
    return false;
  }

  Scalar t = edge13.Dot(plane_normal) * invdet;
  bool found = ray.InRange(t) && (data == NULL || t < data->t);
  if (found && data != NULL) {
    data->element = this;
    data->position = ray.PointAt(t);
    data->normal = vertex1_->normal() * (1 - u - v) + vertex2_->normal() * u
                   + vertex3_->normal() * v;
    data->material = material_;
    data->t = t;
  }
  return found;
}

