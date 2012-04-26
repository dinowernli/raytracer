/*
 * Author: Dino Wernli
 */

#include "camera.h"

#include <cmath>

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

Vector3 Camera::ToWorld(const Vector3& vector) {
  // Execute the matrix multiplication M* [vector; 1] where M =
  // [  right.x,  right.y,  right.z,  0
  //    up.x,     up.y,     up.z,     0
  //    dir.x,    dir.y,    dir.z,    0
  //    0,        0,        0,        1   ].
  // Note that we use [vector; 1] because we are transforming a direction and
  // not a point.

  // TODO(dinow): Possibly precompute these, since they do not depend on vector.
  Vector3 col1(right_.x(), up_.x(), view_.x());
  Vector3 col2(right_.y(), up_.y(), view_.y());
  Vector3 col3(right_.z(), up_.z(), view_.z());

  Vector3 result = vector.x() * col1 + vector.y() * col2 + vector.z() * col3;
  return result.Normalized();
}

Ray Camera::GenerateRay(const Sample& sample) {
  // Add 0.5 to send ray through the pixel center.
  Scalar image_x = sample.x() + 0.5;
  Scalar image_y = sample.y() + 0.5;

  // Convert image coordinates to camera coordinates.
  Scalar tangent = 2 * tan(opening_angle_ * PI / 180.0);
  Scalar factor = tangent / resolution_y_;

  Scalar dx = (image_x - resolution_x_) * factor;
  Scalar dy = (image_y - resolution_y_) * factor;

  // Convert image coordinates to world coordinates.
  Vector3 camera_coords(dx, dy, 1);
  return Ray(position_, ToWorld(camera_coords));
}
