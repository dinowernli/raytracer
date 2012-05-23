/*
 * Can dump an image to disk as a valid BMP file.
 * Author: Dino Wernli
 */

#ifndef BMPEXPORTER_H_
#define BMPEXPORTER_H_

#include <string>

#include "renderer/updatable.h"
#include "util/no_copy_assign.h"

class Image;
class Sampler;

class BmpExporter : public Updatable {
 public:
  BmpExporter(const std::string& file_name);
  virtual ~BmpExporter();
  NO_COPY_ASSIGN(BmpExporter);

  virtual void Ended(const Sampler& sampler);

  // Writes the image to the file.
  void Export(const Image& image);

 private:
  // Full path to the resulting file.
  const std::string file_name_;
};

#endif  /* BMPEXPORTER_H_ */
