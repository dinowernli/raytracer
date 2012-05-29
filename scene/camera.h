/*
 * Represents a camera which can be part of a scene. It supports depth-of-field.
 * Author: Dino Wernli
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cstddef>

#include "util/no_copy_assign.h"
#include "util/numeric.h"
#include "util/point3.h"
#include "util/random.h"
#include "util/ray.h"
#include "util/vector3.h"

class Sample;

class Camera {
 public:
  // The "right" vector is inferred from "view" and "up". The arguments
  // focal_depth and lens_size are only used for depth-of-field. If any of those
  // is negative, no depth-of-field effect is simulated.
  Camera(const Point3& position, const Vector3& view, const Vector3& up,
         Scalar opening_angle, size_t resolution_x, size_t resolution_y,
         Scalar focal_depth = -1, Scalar lens_size = -1);

  virtual ~Camera();
  NO_COPY_ASSIGN(Camera);

  // Returns a ray from the camera origin through sample position.
  Ray GenerateRay(const Sample& sample) const;

  size_t resolution_x() const { return resolution_x_; }
  size_t resolution_y() const { return resolution_y_; }

  const Point3& position() const { return position_; }

  inline bool DepthOfField() const {
    return focal_depth_ >= 0 && lens_size_ >= 0;
  }

 private:
  // This is static so that getting a random number does not mutate the object.
  static Random random_;

  // Computes a new "up" and a "right" vector such that "view", "up" and "right"
  // are orthogonal. This guarantees that the direction of "view" is maintained.
  void ComputeOrientation(const Vector3& view, const Vector3& up);

  // Takes a vector in camera coordinates and transforms it into world
  // coordinates.
  Vector3 ToWorld(const Vector3& vector) const;

  Point3 position_;
  Vector3 view_;  // Ensured to always be of length 1.
  Vector3 up_;  // Ensured to always be of length 1.
  Vector3 right_;  // Ensured to always be of length 1.

  // Angle between the top and the bottom of the image.
  Scalar opening_angle_;

  size_t resolution_x_;
  size_t resolution_y_;

  // The distance from the camera which is in focus.
  Scalar focal_depth_;

  // The size of the lens for DOF.
  Scalar lens_size_;
};

#endif  /* CAMERA_H_ */
