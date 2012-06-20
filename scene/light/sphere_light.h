/*
 * An area light with a sphere geometry.
 *
 * TODO(dinow): Extract a middle class called GeometryLight which contains the
 * intersect code and the geometry pointer.
 * Author: Dino Wernli
 */

#ifndef SPHERE_LIGHT_H_
#define SPHERE_LIGHT_H_

#include "renderer/intersection_data.h"
#include "scene/geometry/sphere.h"
#include "scene/light/light.h"
#include "scene/material.h"
#include "util/ray.h"

class SphereLight : public Light {
 public:
  SphereLight(const Point3& center, Scalar radius, const Color3& color)
      : Light(color), dummy_material_(NULL, NULL, NULL, NULL, 0, 0, 0, 0) {
    // TODO(dinow): Remove this dummy material as soon as sphere is patched.
    sphere_.reset(new Sphere(center, radius, dummy_material_));
  }
  virtual ~SphereLight() {};

  virtual Ray GenerateRay(const Point3& target) const {
    // TODO(dinow): Somehow handle the target being inside the sphere.
    Point3 origin = sphere_->Sample(target);
    Vector3 direction = origin.VectorTo(target);
    Ray ray(origin, direction, EPSILON, direction.Length() - EPSILON);

    IntersectionData data(ray);
    if (Intersect(ray, &data)) {
      // Ray started on the back side of the sphere, take the intersection
      // point as new origin.
      // TODO(dinow): Note that this is a hack because the distribution is no
      // longer completely uniform. However, the difference is so minimal that
      // rejection sampling is just not worth it.
      origin = ray.PointAt(data.t);
      direction = origin.VectorTo(target);
      ray = Ray(origin, direction, EPSILON, direction.Length() - EPSILON);
    }
    return ray;
  }

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const {
    bool result = sphere_->Intersect(ray, data);
    if (result && (data != NULL)) {
      // Make sure the data object knows it intersected a light.
      data->set_light(this);
    }
    return result;
  }

 private:
  // TODO(dinow): Change the sphere to be creatable without material.
  Material dummy_material_;
  std::unique_ptr<Sphere> sphere_;
};

#endif  /* SPHERE_LIGHT_H_ */
