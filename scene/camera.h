/*
 * Represents a camera which can be part of a scene.
 * Author: Dino Wernli
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstddef>

#include "util/numeric.h"
#include "util/point3.h"
#include "util/vector3.h"

class Camera {
 public:
  Camera(const Point3& position, const Vector3& view, const Vector3& up,
         Scalar opening_angle, size_t resolution_x, size_t resolution_y);
  virtual ~Camera();

  size_t resolution_x() const { return resolution_x_; }
  size_t resolution_y() const { return resolution_y_; }

 private:
  Point3 position_;
  Vector3 view_;
  Vector3 up_;
  Scalar opening_angle_;

  size_t resolution_x_;
  size_t resolution_y_;
};

#endif  /* CAMERA_H_ */
