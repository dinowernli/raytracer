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
 * A special type of texture which maps a color to each pair of 2D texture
 * coordinates.
 * Author: Dino Wernli
 */

#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_

#include <glog/logging.h>

#include "renderer/intersection_data.h"
#include "scene/texture/texture.h"

class Texture2D : public Texture {
 public:
  virtual ~Texture2D() {}
  virtual Color3 Evaluate(const IntersectionData& data) const {
    CHECK(data.HasTextureCoordinates()) << "Attempted to evaluate 2D texture "
                                        << "without 2D texture coordinates";
    return Evaluate2D(data);
  }

 protected:
  virtual Color3 Evaluate2D(const IntersectionData& data) const = 0;
};

#endif  /* TEXTURE_2D_H_ */
