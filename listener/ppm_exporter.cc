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
 * Author: Dino Wernli
 */

#include "listener/ppm_exporter.h"

#include <glog/logging.h>
#include <fstream>

#include "renderer/sampler/sampler.h"
#include "util/color3.h"

// Transforms an intensity in range [0, 1] to an integer in
// {0, ..., kMaxPixelValue}.
unsigned int ScaleIntensity(Intensity intensity) {
  return intensity * PpmExporter::kMaxPixelValue;
}

PpmExporter::PpmExporter(const std::string& file_name) : file_name_(file_name) {
}

PpmExporter::~PpmExporter() {
}

void PpmExporter::Ended(const Sampler& sampler) {
  if (sampler.image().SizeX() == 0 || sampler.image().SizeY() == 0) {
    LOG(INFO) << "Empty image, not exporting to file: " << file_name_;
    return;
  }
  Export(sampler.image());
}

void PpmExporter::Export(const Image& image) {
  LOG(INFO) << "Exporting image to file: " << file_name_;
  std::ofstream file_stream(file_name_);

  file_stream << kMagicNumber << std::endl;
  file_stream << image.SizeX() << " " << image.SizeY() << std::endl;
  file_stream << kMaxPixelValue << std::endl;

  for (size_t y = 0; y < image.SizeY(); ++y) {
    for (size_t x = 0; x < image.SizeX(); ++x) {
      // The image stores the bottom left pixel as (0, 0), we need to start with
      // the top left pixel. So flip the y coordinate.
      const Color3& color = image.PixelAt(x, image.SizeY() - y - 1);
      file_stream << ScaleIntensity(color.r()) << " "
                  << ScaleIntensity(color.g()) << " "
                  << ScaleIntensity(color.b()) << " ";
    }
    file_stream << std::endl;
  }
  file_stream << std::endl;
  file_stream.close();
}

const size_t PpmExporter::kMaxPixelValue = 255;

const std::string PpmExporter::kMagicNumber = "P3";
