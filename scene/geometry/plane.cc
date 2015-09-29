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
 * Author: Dino Wernli
 */

#include "scene/geometry/plane.h"

#include "renderer/intersection_data.h"
#include "util/ray.h"

Plane::Plane(const Point3& point, const Vector3& normal,
             const Material& material)
    : Element(material), point_(point), normal_(normal.Normalized()) {
  if (normal_.x() != 0 || normal_.y() != 0) {
    tangent_ = normal_.Cross(Vector3(0, 0, 1)).Normalized();
  } else {
    tangent_ = normal_.Cross(Vector3(0, 1, 0)).Normalized();
  }
  bitangent_ = normal_.Cross(tangent_);
}

Plane::~Plane() {
}

bool Plane::Intersect(const Ray& ray, IntersectionData* data) const {
  Scalar denominator = ray.direction().Dot(normal_);
  Scalar t = - normal_.Dot(point_.VectorTo(ray.origin())) / denominator;

  bool found = ray.InRange(t) && (data == NULL || t < data->t);
  if (found && data != NULL) {
    data->set_element(this);
    data->position = ray.PointAt(t);
    data->normal = normal_;
    data->material = &material();
    data->t = t;

    Vector3 point_to_hit = point_.VectorTo(data->position);
    Scalar s = point_to_hit.Dot(tangent_);
    Scalar t = point_to_hit.Dot(bitangent_);
    data->texture_coordinate.s = s - floor(s);
    data->texture_coordinate.t = t - floor(t);
  }
  return found;
}
