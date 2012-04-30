/*
 * Author: Dino Wernli
 */

#include "scene/geometry/plane.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Plane::Plane(const Point3& point, const Vector3& normal,
             const Material* material)
    : point_(point), normal_(normal.Normalized()), material_(material) {
}

Plane::~Plane() {
}

bool Plane::Intersect(const Ray& ray, IntersectionData* data) {
  Scalar denominator = ray.direction().Dot(normal_);
  Scalar t = - normal_.Dot(point_.VectorTo(ray.origin())) / denominator;

  bool found = ray.InRange(t) && (data == NULL || t < data->t);
  if (found && data != NULL) {
    data->element = this;
    data->position = ray.PointAt(t);
    data->normal = normal_;
    data->material = material_;
    data->t = t;
  }
  return found;
}
