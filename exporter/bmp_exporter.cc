/*
 * Author: Dino Wernli
 */

#include <iostream>

#include "bmp_exporter.h"

#include "renderer/image.h"

BmpExporter::BmpExporter() {
}

BmpExporter::~BmpExporter() {
}

void BmpExporter::Update(const Image& image) {
  // TODO(dinow): Actually export the image.
  std::cout << "Updated." << std::endl;
}
