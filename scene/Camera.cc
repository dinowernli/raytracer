/*
 * Author: Dino Wernli
 */

#include "Camera.h"

Camera::Camera(const Point3& position, 
               const Vector3& view, 
               const Vector3& up, 
               Scalar opening_angle)
    : position_(position), 
	view_(view), 
    up_(up), 
    opening_angle_(opening_angle) {
}

Camera::~Camera() {
}

size_t Camera::resolution_x() const {
  return resolution_x_;
}

size_t Camera::resolution_y() const {
  return resolution_y_;
}
