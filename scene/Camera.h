/*
 * Represents a camera which can be part of a scene.
 * Author: Dino Wernli
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstddef>

#include "util/Numeric.h"
#include "util/Point3.h"
#include "util/Vector3.h"

class Camera {
 public:
  Camera(const Point3& position, const Vector3& view, const Vector3& up,
         Scalar opening_angle);
  virtual ~Camera();

  size_t resolution_x() const;
  size_t resolution_y() const;

 private:
  size_t resolution_x_;
  size_t resolution_y_;

  Point3 position_;
  Vector3 view_;
  Vector3 up_;
  Scalar opening_angle_;
};

#endif  /* CAMERA_H_ */
