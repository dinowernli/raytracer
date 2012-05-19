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
  // Only export if the image is done.
  if (!sampler.IsDone()) {
    return;
  }

  const Image& image = sampler.image();
  if (image.SizeX() == 0 && image.SizeY() == 0) {
    LOG(INFO) << "Empty image, not exporting to file: " << file_name_;
    return;
  }

  LOG(INFO) << "Exporting image to file: " << file_name_;
  std::ofstream file_stream(file_name_);

  file_stream << kMagicNumber << std::endl;
  file_stream << image.SizeX() << " " << image.SizeY() << std::endl;
  file_stream << kMaxPixelValue << std::endl;

  for (size_t y = 0; y < image.SizeY(); ++y) {
    for (size_t x = 0; x < image.SizeX(); ++x) {
      const Color3& color = image.PixelAt(x, y);
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
