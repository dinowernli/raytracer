/*
 * Author: Dino Wernli
 */

#ifndef PPM_EXPORTER_H_
#define PPM_EXPORTER_H_

#include "renderer/updatable.h"

#include <string>

class Image;

class PpmExporter : public Updatable{
 public:
  PpmExporter(std::string file_name);
  virtual ~PpmExporter();
  virtual void Update(const Image& image);

  static size_t kMaxPixelValue;
  static std::string kMagicNumber;

 private:
  std::string file_name_;
};

#endif  /* PPM_EXPORTER_H_ */
