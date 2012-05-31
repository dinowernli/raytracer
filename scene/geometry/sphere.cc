/*
 * Author: Dino Wernli
 */

#include "scene/geometry/sphere.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

// Convenience method which creates a bounding box for a circle.
static inline BoundingBox* MakeBoundingBox(const Point3& center, Scalar radius){
  Vector3 offset(radius, radius, radius);
  return &(new BoundingBox(center + offset))->Include(center - offset);
}

Sphere::Sphere(const Point3& center, Scalar radius, const Material& material)
    : Element(material, MakeBoundingBox(center, radius)), center_(center),
      radius_(radius) {
}

Sphere::~Sphere() {
}

// Solves the quadratic equation for ray-sphere intersection.
bool Sphere::Intersect(const Ray& ray, IntersectionData* data) const {
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
  Scalar t = 0;

  // Find out which t is closer.
  if (ray.InRange(t1) && (!found || t1 < t)) {
    t = t1;
    found = true;
  }
  if (ray.InRange(t2) && (!found || t2 < t)) {
    t = t2;
    found = true;
  }

  if (data != NULL && found) {
    data->t = t;
    data->position = ray.PointAt(t);
    data->set_element(this);
    data->material = &material();
    data->normal = center_.VectorTo(data->position).Normalized();
  }
  return found;
}
