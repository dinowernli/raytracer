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
 * Represents a generic light object which has a color and can shoot rays of
 * light.
 * Author: Dino Wernli
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "util/color3.h"

class Point3;
class Ray;

class Light {
 public:
  Light(const Color3& color) : color_(color) {}
  virtual ~Light() {}

  // Generates a ray of light from the source to target.
  virtual Ray GenerateRay(const Point3& target) const = 0;

  // Returns the color of light coming from this source.
  const Color3& color() const { return color_; }

  // Returns whether the ray intersects this light source. If this returns true
  // and data != NULL, information about the first intersection is stored.
  virtual bool Intersect(const Ray& ray,
                         IntersectionData* data = NULL) const = 0;

 private:
  Color3 color_;
};


#endif /* LIGHT_H_ */
