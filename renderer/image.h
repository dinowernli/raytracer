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
 * A container for the raw pixels produced by the render loop.
 * Author: Dino Wernli
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <glog/logging.h>
#include <limits>
#include <memory>
#include <vector>

#include "util/color3.h"

class Image {
 public:
  // Initially, the image is completely black.
  Image(size_t width, size_t height)
      : size_x_(width), size_y_(height),
        pixels_(kNumberOfChannels * size_x_ * size_y_, 0.0) {
  }
  ~Image() {};

  // Sets the color at the specified position in the pixel buffer. Does nothing
  // if the position is invalid. The entry (0, 0) is the bottom left pixel.
  void PutPixel(const Color3& color, size_t x, size_t y) {
    if (x >= size_x_ || y >= size_y_) {
      LOG(WARNING) << "Attempted to set invalid pixel [" << x << ", " << y
                   << "] in image of size [" << size_x_ << ", " << size_y_
                   << "]";
      return;
    }
    pixels_[kNumberOfChannels * (y * size_x_ + x) + 0] = color.r();
    pixels_[kNumberOfChannels * (y * size_x_ + x) + 1] = color.g();
    pixels_[kNumberOfChannels * (y * size_x_ + x) + 2] = color.b();
  }

  // Returns the color at position (x, y) of the image. The entry (0, 0) is the
  // bottom left pixel. Returns a default color if the position is invalid.
  const Color3 PixelAt(size_t x, size_t y) const {
    if (x >= size_x_ || y >= size_y_) {
      LOG(WARNING) << "Attempted access to invalid pixel [" << x << ", " << y
                   << "] in image of size [" << size_x_ << ", " << size_y_
                   << "]";
      return Color3();
    }
    size_t pos = kNumberOfChannels * (y * size_x_ + x);
    return Color3(pixels_[pos], pixels_[pos + 1], pixels_[pos + 2]);
  }

  Intensity MaxIntensity() const {
    Intensity result = -std::numeric_limits<Intensity>::infinity();
    for (size_t x = 0; x < SizeX(); ++x) {
      for (size_t y = 0; y < SizeY(); ++y) {
        const Color3& color = PixelAt(x, y);
        result = std::max(result, color.r());
        result = std::max(result, color.g());
        result = std::max(result, color.b());
      }
    }
    DVLOG(1) << "Computed maximum as: " << result;
    return result;
  }

  // Multiplies all the intensities by factor.
  void Scale(const Intensity& factor) {
    for (size_t x = 0; x < SizeX(); ++x) {
      for (size_t y = 0; y < SizeY(); ++y) {
        PutPixel(factor * PixelAt(x, y), x, y);
      }
    }
  }

  // Is guaranteed not to change throughout the lifetime of this object.
  const Intensity* RawData() const { return &pixels_[0]; }

  const size_t SizeX() const { return size_x_; }
  const size_t SizeY() const { return size_y_; }

 private:
  // We have 3 color channels.
  static const size_t kNumberOfChannels = 3;

  const size_t size_x_;
  const size_t size_y_;

  // Stores the raw colors of the image.
  std::vector<Intensity> pixels_;
};

#endif  /* IMAGE_H_ */
