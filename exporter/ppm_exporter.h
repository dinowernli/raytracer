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

  const static size_t kMaxPixelValue;
  const static std::string kMagicNumber;

 private:
  const std::string file_name_;
};

#endif  /* PPM_EXPORTER_H_ */
