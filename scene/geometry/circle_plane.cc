/*
 * Author: Dino Wernli
 */

#include "circle_plane.h"

#include "renderer/intersection_data.h"

CirclePlane::CirclePlane(const Point3& point, const Vector3& normal,
                         Scalar radius, const Material& ring_material,
                         const Material& main_material)
    : Plane(point, normal, main_material), radius_(radius),
      ring_material_(ring_material) {
}

CirclePlane::~CirclePlane() {
}

bool CirclePlane::Intersect(const Ray& ray, IntersectionData* data) const {
  bool found = Plane::Intersect(ray, data);
  if (data != NULL && data->element() == this) {
    size_t dist = Point3::Distance(data->position, point()) / radius_;
    if (dist % 2 == 0) {
      data->material = &ring_material_;
    }
  }
  return found;
}
