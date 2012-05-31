/*
 * An area light with a sphere geometry.
 *
 * TODO(dinow): Extract a middle class called GeometryLight which contains the
 * intersect code and the geometry pointer.
 * Author: Dino Wernli
 */

#ifndef SPHERE_LIGHT_H_
#define SPHERE_LIGHT_H_

#include "scene/geometry/sphere.h"
#include "util/random.h"

class SphereLight : public Light {
 public:
  SphereLight(const Point3& center, Scalar radius, const Color3& color)
      : Light(color), sphere_(new Sphere(center, radius)) {
  }
  virtual ~SphereLight() {};

  virtual Ray GenerateRay(const Point3& target) const {
    // TODO(dinow): Generate a ray from a random point on the proper half of the
    // sphere to target.
    return Ray;
  }

  virtual bool Intersect(const Ray& ray, IntersectionData* data) const {
    bool result = sphere_->Intersect(ray, data);
    if (result) {
      // Make sure the data object knows it intersected a light.
      data->set_light(this);
    }
    return result;
  }

 private:
  std::unique_ptr<Sphere> sphere_;
  Random random_;
};

#endif  /* SPHERE_LIGHT_H_ */
