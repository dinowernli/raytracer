/*
 * Author: Dino Wernli
 */

#include "Camera.h"

Camera::Camera(const Point3& position, const Vector3& view, const Vector3& up,
               Scalar opening_angle, size_t resolution_x, size_t resolution_y)
    : position_(position), view_(view), up_(up), opening_angle_(opening_angle),
      resolution_x_(resolution_x), resolution_y_(resolution_y) {
}

Camera::~Camera() {
}
