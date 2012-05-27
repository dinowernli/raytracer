/*
 * Author: Dino Wernli
 */

#include "camera.h"

#include <cmath>
#include <glog/logging.h>

#include "renderer/sampler/sample.h"

Camera::Camera(const Point3& position, const Vector3& view, const Vector3& up,
               Scalar opening_angle, size_t resolution_x, size_t resolution_y)
    : position_(position), opening_angle_(opening_angle),
      resolution_x_(resolution_x), resolution_y_(resolution_y) {
  ComputeOrientation(view, up);
}

Camera::~Camera() {
}

void Camera::ComputeOrientation(const Vector3& view, const Vector3& up) {
  view_ = view.Normalized();
  right_ = (view_.Cross(up)).Normalized();
  up_ = (right_.Cross(view_)).Normalized();
}

Vector3 Camera::ToWorld(const Vector3& vector) const {
  // Execute the matrix multiplication M * [vector; 1] where M =
  // [ right.x,  up.x,  view.x,  0
  //   right.y,  up.y,  view.y,  0
  //   right.z,  up.z,  view.z,  0
  //   0,        0,     0,       1 ].
  // Note that we use [vector; 1] because we are transforming a direction and
  // not a point.
  Vector3 result = vector.x() * right_ + vector.y() * up_ + vector.z() * view_;
  return result.Normalize();
}

Ray Camera::GenerateRay(const Sample& sample) const {
  // The sample (0, 0) represents the bottom left pixel. Note that we do not add
  // 0.5 here because we expect the offset to already contian this adjustment.
  Scalar image_x = sample.x() + sample.offset_x();
  Scalar image_y = sample.y() + sample.offset_y();

  // Convert image coordinates to camera coordinates.
  Scalar tangent = 2 * tan(opening_angle_ * PI / 180.0);
  Scalar factor = tangent / resolution_y_;
  Scalar dx = (image_x - resolution_x_ / 2.0) * factor;
  Scalar dy = (image_y - resolution_y_ / 2.0) * factor;

  // Convert image coordinates to world coordinates.
  Vector3 camera_coords(dx, dy, 1);
  return Ray(position_, ToWorld(camera_coords));
}
