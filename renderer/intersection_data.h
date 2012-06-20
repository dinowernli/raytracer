/*
 * Contains the information about an intersection.
 * Author: Dino Wernli
 */

#ifndef INTERSECTIONDATA_H_
#define INTERSECTIONDATA_H_

#include "util/point3.h"
#include "util/ray.h"

class Element;
class Light;
class Material;

struct TextureCoordinates {
  // After initialization, Valid() will be false.
  TextureCoordinates() { Invalidate(); }
  bool Valid() const { return s >= 0 && t >= 0; }
  void Set(Scalar ss, Scalar tt) { s = ss; t = tt; }
  void Invalidate() { Set(-1, -1); }

  Scalar s, t;
};

class IntersectionData {
 public:
  // Builds a new struct for this specific ray. The ray is necessary in order to
  // initialize "t" to the maximum "t" allowed by the ray.
  IntersectionData(const Ray& ray)
      : t(ray.max_t()), material(NULL), element_(NULL), light_(NULL) {}

  bool IntersectedElement() const { return element_ != NULL; }
  bool IntersectedLight() const { return light_ != NULL; }

  void set_element(const Element* element) {
    element_ = element;
    light_ = NULL;
  }

  void set_light(const Light* light) {
    element_ = NULL;
    light_ = light;
  }

  // Returns whether or not the intersection has valid 2D texture coordinates.
  // Note that 3D texture coordinates are just given by position.
  bool HasTextureCoordinates() const { return texture_coordinates.Valid(); }

  // Returns the intersected element or NULL if it is not an element.
  const Element* element() const { return element_; }

  // Returns the intersected light or NULL if it is not a light.
  const Light* light() const { return light_; }

  Scalar t;
  Point3 position;
  Vector3 normal;
  const Material* material;
  TextureCoordinates texture_coordinates;

 private:
  const Element* element_;
  const Light* light_;
};

#endif /* INTERSECTIONDATA_H_ */
