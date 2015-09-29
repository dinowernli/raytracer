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
 * A 2D texture which produces a checkerboard.
 * Author: Dino Wernli
 */

#ifndef CHECKERBOARD_H_
#define CHECKERBOARD_H_

#include <glog/logging.h>

#include "scene/texture/texture_2d.h"

class Checkerboard : public Texture2D {
 public:
  // Multiplies the supplied length by 2 in order to get the desired alternating
  // pattern.
  // TODO(dinow): In order to support arbitrary lengths, the tex coordinates
  // should not be in [0, 1] but in [-inf, inf].
  Checkerboard(Color3 first = Color3(0, 0, 0),
               Color3 second = Color3(1, 1, 1), size_t length = 1)
      : first_(first), second_(second), length_(2 * length) {}
  virtual ~Checkerboard() {}

 protected:
  virtual Color3 Evaluate2D(const IntersectionData& data) const {
    long sss = floor(data.texture_coordinate.s * length_);
    long ttt = floor(data.texture_coordinate.t * length_);
    if ((sss & 1) == (ttt & 1)) {
      return first_;
    } else {
      return second_;
    }
  }

 private:
  Color3 first_;
  Color3 second_;
  size_t length_;
};


#endif  /* CHECKERBOARD_H_ */
