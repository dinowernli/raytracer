/*
 * A container for the raw pixels produced by the render loop. Can export
 * itself in different file formats.
 * Author: Dino Wernli
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <memory>
#include <vector>

class Color3;

class Image {
 public:
  // Initially, the image is completely black.
  Image(size_t width, size_t height);
  virtual ~Image();

  // Sets the color at the specified position in the pixel buffer.
  void PutPixel(const Color3& color, size_t x, size_t y);

 private:
  // Stores the raw colors of the image.
  std::vector<std::vector<Color3> > pixels_;
};

#endif  /* IMAGE_H_ */
