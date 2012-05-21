/*
 * A container for the raw pixels produced by the render loop. Can export
 * itself in different file formats.
 * Author: Dino Wernli
 */

#ifndef IMAGE_H_
#define IMAGE_H_

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
  // if the position is invalid.
  void PutPixel(const Color3& color, size_t x, size_t y) {
    if (x >= size_x_ || y >= size_y_) {
      LOG(WARNING) << "Attempted to set invalid pixel [" << x << ", " << y
                   << "] in image of size [" << size_x_ << ", " << size_y_
                   << "]";
      return;
    }

    // TODO(dinow): Y-axis flipped here and in exporter. Potentially remove
    // both.
    size_t yy = size_y_ - y - 1;
    pixels_[kNumberOfChannels * (yy * size_x_ + x) + 0] = color.r();
    pixels_[kNumberOfChannels * (yy * size_x_ + x) + 1] = color.g();
    pixels_[kNumberOfChannels * (yy * size_x_ + x) + 2] = color.b();
  }

  // Returns the color at position (x, y) of the image. The entry (0, 0) is the
  // top left pixel. Returns a default color if the position is invalid.
  const Color3 PixelAt(size_t x, size_t y) const {
    if (x >= size_x_ || y >= size_y_) {
      LOG(WARNING) << "Attempted access to invalid pixel [" << x << ", " << y
                   << "] in image of size [" << size_x_ << ", " << size_y_
                   << "]";
      return Color3();
    }

    // TODO(dinow): Y-axis flipped here and in exporter. Potentially remove
    // both.
    size_t yy = size_y_ - y - 1;
    size_t pos = kNumberOfChannels * (yy * size_x_ + x);
    return Color3(pixels_[pos], pixels_[pos + 1], pixels_[pos + 2]);
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
