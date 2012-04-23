/*
 * An object which can dump and image to disk as a valid BMP file.
 * Author: Dino Wernli
 */

#ifndef BMPEXPORTER_H_
#define BMPEXPORTER_H_

#include "renderer/Updatable.h"

class Image;

class BmpExporter : public Updatable {
public:
  BmpExporter();
  virtual ~BmpExporter();

  virtual void Update(const Image& image);
};

#endif  /* BMPEXPORTER_H_ */
