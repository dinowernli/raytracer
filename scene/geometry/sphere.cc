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

  Scalar a = ray.direction().SquaredLength();
  Scalar b = 2 * (ray.direction().Dot(center_to_origin));
  Scalar c = center_to_origin.SquaredLength() - radius_ * radius_;

  Scalar discrim = b * b - 4 * a * c;
  if(discrim < 0.) {
    return false;
  }
  Scalar root_discrim = sqrt(discrim);

  Scalar q = b < 0 ? -0.5 * (b - root_discrim) : -0.5 * (b + root_discrim);
  Scalar t1 = q / a;
  Scalar t2 = c / q;
  if (t1 > t2) {
    std::swap(t1, t2);
  }

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

    Vector3 center_to_hit = center_.VectorTo(data->position);
    center_to_hit.Normalize();
    data->normal = center_to_hit.Normalized();

    Scalar x = center_to_hit.x() != 0 ? center_to_hit.x() : EPSILON;
    Scalar y = center_to_hit.y() != 0 ? center_to_hit.y() : EPSILON;
    Scalar phi = atan2(y, x);
    if (phi < 0) phi += 2 * PI;
    data->texture_coordinate.s = phi / (2 * PI);

    Scalar zr = center_to_hit.z();
    zr = zr < -1 ? -1 : zr;
    zr = zr > 1 ? 1 : zr;
    data->texture_coordinate.t = acos(zr) / PI;
  }
  return found;
}

Point3 Sphere::Sample(const Point3& point) const {
  // First, sample a unit vector in a random direction.
  Scalar z = random_.Get(-1, 1);
  Scalar phi = random_.Get(0, 2 * PI);

  // We know that z = sin(theta), so sqrt(1 - z^2) = cos(theta).
  Scalar cos_theta = sqrt(1 - z * z);
  Scalar x = cos_theta * cos(phi);
  Scalar y = cos_theta * sin(phi);

  Vector3 dir (x, y, z);
  Vector3 normal = center_.VectorTo(point);
  if (dir.Dot(normal) < 0) {
    dir = -1 * dir;
  }
  return center_ + radius_ * dir;
}
