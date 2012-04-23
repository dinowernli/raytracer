/*
 * Author: Dino Wernli
 */

#include "Image.h"

#include "util/Color3.h"

Image::Image(size_t width, size_t height)
    : pixels_(width, std::vector<Color3>(height)) {
}

Image::~Image() {
}
