/*
 * An object which can dump and image to disk as a valid BMP file.
 * Author: Dino Wernli
 */

#ifndef BMPEXPORTER_H_
#define BMPEXPORTER_H_

#include "renderer/updatable.h"

class Sampler;

class BmpExporter : public Updatable {
 public:
  BmpExporter();
  virtual ~BmpExporter();

  virtual void Update(const Sampler& sampler);
};

#endif  /* BMPEXPORTER_H_ */
