/*
 * Author: Dino Wernli
 */

#include "scene/geometry/plane.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Plane::Plane(const Point3& point, const Vector3& normal)
    : point_(point), normal_(normal.Normalized()) {
}

Plane::~Plane() {
}

bool Plane::Intersect(const Ray& ray, IntersectionData* data) {
  Scalar t = - normal_.Dot(point_.VectorTo(ray.origin())) /
      ray.direction().Dot(normal_);
  if (t < data->t && t >= ray.min_t() && t <= ray.max_t()) {
    data->element = this;
    data->position = ray.PointAt(t);
    data->t = t;
    return true;
  }
  return false;
}

