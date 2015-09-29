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

#include "circle_plane.h"

#include "renderer/intersection_data.h"

CirclePlane::CirclePlane(const Point3& point, const Vector3& normal,
                         Scalar radius, const Material& ring_material,
                         const Material& main_material)
    : Plane(point, normal, main_material), radius_(radius),
      ring_material_(ring_material) {
}

CirclePlane::~CirclePlane() {
}

bool CirclePlane::Intersect(const Ray& ray, IntersectionData* data) const {
  bool found = Plane::Intersect(ray, data);
  if (data != NULL && data->element() == this) {
    size_t dist = Point3::Distance(data->position, point()) / radius_;
    if (dist % 2 == 0) {
      data->material = &ring_material_;
    }
  }
  return found;
}
