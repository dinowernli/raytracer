/*
 * Author: Dino Wernli
 */

#ifndef PPM_EXPORTER_H_
#define PPM_EXPORTER_H_

#include <string>

#include "renderer/updatable.h"
#include "util/no_copy_assign.h"

class Sampler;

class PpmExporter : public Updatable{
 public:
  PpmExporter(const std::string& file_name);
  virtual ~PpmExporter();
  NO_COPY_ASSIGN(PpmExporter);

  virtual void Ended(const Sampler& sampler);

  const static size_t kMaxPixelValue;
  const static std::string kMagicNumber;

 private:
  const std::string file_name_;
};

#endif  /* PPM_EXPORTER_H_ */
