/*
 * Author: Dino Wernli
 */

#include "image.h"

#include "util/color3.h"

Image::Image(size_t width, size_t height)
    : pixels_(width, std::vector<Color3>(height)) {
}

Image::~Image() {
}

void Image::PutPixel(const Color3& color, size_t x, size_t y) {
  pixels_[x][y] = color;
}
