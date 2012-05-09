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

void BmpExporter::Update(const Sampler& sampler) {
  // Only export if the image is done.
  if (!sampler.IsDone()) {
    return;
  }

  // TODO(dinow): Move the magic constants out of this code.

  LOG(INFO) << "Exporting image " << file_name_;

  const Image& image = sampler.image();
  std::ofstream file_stream(file_name_, std::ofstream::binary);

  int w = image.SizeX();
  int h = image.SizeY();

  int filesize = 54 + 3*w*h;

  DVLOG(1) << "Filesize: " << filesize;

  char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

  bmpfileheader[ 2] = (char)(filesize);
  bmpfileheader[ 3] = (char)(filesize>> 8);
  bmpfileheader[ 4] = (char)(filesize>>16);
  bmpfileheader[ 5] = (char)(filesize>>24);

  bmpinfoheader[ 4] = (char)(w);
  bmpinfoheader[ 5] = (char)(w>> 8);
  bmpinfoheader[ 6] = (char)(w>>16);
  bmpinfoheader[ 7] = (char)(w>>24);
  bmpinfoheader[ 8] = (char)(h);
  bmpinfoheader[ 9] = (char)(h>> 8);
  bmpinfoheader[10] = (char)(h>>16);
  bmpinfoheader[11] = (char)(h>>24);

  file_stream.write(bmpfileheader, 14);
  file_stream.write(bmpinfoheader, 40);

  int extrabytes = (4 - (w * 3) % 4) % 4;
  char padding[3] = {0, 0, 0};

  for (int row = 0; row < h; ++row) {
    for (int col = 0; col < w; ++col) {
      const Color3& pixel = image.PixelAt(col, h - row - 1);
      char buffer[3];
      buffer[0] = (char)(pixel.b() * 255);
      buffer[1] = (char)(pixel.g() * 255);
      buffer[2] = (char)(pixel.r() * 255);
      file_stream.write(buffer, 3);
    }
    file_stream.write(padding, extrabytes);
  }

  file_stream.close();
}
