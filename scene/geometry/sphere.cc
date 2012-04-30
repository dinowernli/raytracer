/*
 * Author: Dino Wernli
 */

#include "scene/geometry/sphere.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Sphere::Sphere(const Point3& center, Scalar radius, const Material* material)
    : center_(center), radius_(radius), material_(material) {
}

Sphere::~Sphere() {
}

// Solves the quadratic equation for ray-sphere intersection.
bool Sphere::Intersect(const Ray& ray, IntersectionData* data) {
  Vector3 center_to_origin = center_.VectorTo(ray.origin());

  Scalar aa = 2 * ray.direction().SquaredLength();
  Scalar b = 2 * (ray.direction().Dot(center_to_origin));
  Scalar c = center_to_origin.SquaredLength() - radius_ * radius_;

  Scalar under_root = b*b - 2 * aa * c;
  if (under_root < 0) {
    return false;
  }

  Scalar t1 = (-b - sqrt(under_root)) / aa;
  Scalar t2 = (-b + sqrt(under_root)) / aa;

  bool found = false;
  Scalar t;

  // Find out which t is closer.
  if (ray.InRange(t1) && (data == NULL || t1 < data->t)) {
    t = t1;
    found = true;
  }
  if (ray.InRange(t2) && (data == NULL || t2 < data->t)) {
    t = t2;
    found = true;
  }

  if (data != NULL && found) {
    data->t = t;
    data->position = ray.PointAt(t);
    data->element = this;
    data->material = material_;
  }
  return found;
}
