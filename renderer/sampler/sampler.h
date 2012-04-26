/*
 * Represents an objects which is in charge of generating samples for a given
 * camera. The samples can then be traced and passed back to the sampler once
 * they contain a color. The sampler also manages the process of inserting
 * colored samples into an image.
 * Author: Dino Wernli
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <memory>

#include "renderer/image.h"
#include "scene/camera.h"

class Sample;

class Sampler {
 public:
  virtual ~Sampler() { };

  // Prepares for generating samples for the given camera. Must be called
  // before starting to fetch samples.
  virtual void Init(const Camera& camera) {
    image_.reset(new Image(camera.resolution_x(), camera.resolution_y()));
  }

  virtual Sample* NextSample() = 0;

 private:
  std::unique_ptr<Image> image_;
};

#endif  /* SAMPLER_H_ */
