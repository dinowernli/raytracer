/*
 * Represents a generic light object which has a color and can shoot rays of
 * light.
 * Author: Dino Wernli
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "util/color3.h"

class Point3;
class Ray;

class Light {
 public:
  Light(const Color3& color) : color_(color) {}
  virtual ~Light() {}

  // Generates a ray of light from the source to target.
  virtual Ray GenerateRay(const Point3& target) const = 0;

  // Returns the color of light coming from this source.
  const Color3& color() const { return color_; }

  // Returns whether the ray intersects this light source. If this returns true
  // and data != NULL, information about the first intersection is stored.
  virtual bool Intersect(const Ray& ray,
                         IntersectionData* data = NULL) const = 0;

 private:
  Color3 color_;
};


#endif /* LIGHT_H_ */
