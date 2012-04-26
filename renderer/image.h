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
  Image(size_t width, size_t height) : pixels_(width,
                                               std::vector<Color3>(height)) {
  }
  ~Image() { };

  // Sets the color at the specified position in the pixel buffer.
  void PutPixel(const Color3& color, size_t x, size_t y) {
    pixels_[x][y] = color;
  }

  // Returns the color at position (x, y) of the image.
  const Color3& PixelAt(size_t x, size_t y) const {
    return pixels_[x][y];
  }

  const size_t SizeX() const { return pixels_.size(); }
  const size_t SizeY() const {
    if (SizeX() == 0) {
      return 0;
    }
    return pixels_[0].size();
  }

 private:
  // Stores the raw colors of the image. The entry (0, 0) is the top left pixel.
  std::vector<std::vector<Color3> > pixels_;
};

#endif  /* IMAGE_H_ */
