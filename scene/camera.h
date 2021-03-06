// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

  inline Scalar focal_depth() const { return focal_depth_; }

 private:
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

  mutable Random random_;
};

#endif  /* CAMERA_H_ */
