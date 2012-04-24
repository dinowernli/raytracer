/*
 * Represents a dimensionless point light with a color and a position.
 * Author: Dino Wernli
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "scene/Light.h"
#include "util/Point3.h"
#include "util/Ray.h"

class PointLight : public Light {
 public:
  PointLight(const Point3& position, const Color3& color)
      : Light(color), position_(position) {
  }
  virtual ~PointLight() {};

  Ray GenerateRay(const Point3& target) {
    return Ray(position_, position_.VectorTo(target));
  }

 private:
  Point3 position_;
};


#endif /* POINTLIGHT_H_ */
