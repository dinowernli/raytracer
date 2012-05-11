/*
 * Author: Dino Wernli
 */

#include "listener/bmp_exporter.h"

#include <fstream>

#include "renderer/sampler/sampler.h"
#include "util/color3.h"

BmpExporter::BmpExporter(const std::string& file_name) : file_name_(file_name) {
}

BmpExporter::~BmpExporter() {
}

static void WriteHeader(size_t width, size_t height, std::ofstream* stream) {
  const size_t filesize = 54 + 3 * width * height;
  char file_header[14] = {'B','M',0,0,0,0,0,0,0,0,54,0,0,0};
  char info_header[40] = {40,0,0,0,0,0,0,0,0,0,0,0,1,0,24,0};

  file_header[2] = (char)(filesize);
  file_header[3] = (char)(filesize>> 8);
  file_header[4] = (char)(filesize>>16);
  file_header[5] = (char)(filesize>>24);

  info_header[4] = (char)(width);
  info_header[5] = (char)(width>> 8);
  info_header[6] = (char)(width>>16);
  info_header[7] = (char)(width>>24);
  info_header[8] = (char)(height);
  info_header[9] = (char)(height>> 8);
  info_header[10] = (char)(height>>16);
  info_header[11] = (char)(height>>24);

  stream->write(file_header, 14).write(info_header, 40);
}

void BmpExporter::Update(const Sampler& sampler) {
  // Only export if the image is done.
  if (!sampler.IsDone()) {
    return;
  }

  const Image& image = sampler.image();
  const size_t width = image.SizeX();
  const size_t height = image.SizeY();

  if (height == 0 && width == 0) {
    LOG(INFO) << "Empty image, not exporting to file: " << file_name_;
    return;
  }

  LOG(INFO) << "Exporting image " << file_name_;
  std::ofstream file_stream(file_name_, std::ofstream::binary);
  WriteHeader(width, height, &file_stream);

  // Due to alignment, we must append the following number of bytes as padding.
  const size_t extra_bytes = (4 - (width * 3) % 4) % 4;
  char padding[3] = {0, 0, 0};

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      const Color3& pixel = image.PixelAt(x, height - y - 1);
      char buffer[3] = { (char)(pixel.b() * 255),
                         (char)(pixel.g() * 255),
                         (char)(pixel.r() * 255) };
      file_stream.write(buffer, 3);
    }
    file_stream.write(padding, extra_bytes);
  }
  file_stream.close();
}
