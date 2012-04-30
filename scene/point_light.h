/*
 * Represents a dimensionless point light with a color and a position.
 * Author: Dino Wernli
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "scene/light.h"
#include "util/point3.h"
#include "util/ray.h"

class PointLight : public Light {
 public:
  PointLight(const Point3& position, const Color3& color)
      : Light(color), position_(position) {
  }
  virtual ~PointLight() {};

  const Point3& position() const { return position_; }

  virtual Ray GenerateRay(const Point3& target) const {
    Vector3 direction = position_.VectorTo(target);

    // Subtract epsilon to prevent the ray from intersecting with at "position".
    return Ray(position_, direction, 0, direction.Length() - EPSILON);
  }

 private:
  Point3 position_;
};


#endif /* POINTLIGHT_H_ */
