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
  // before starting to fetch samples. If the passed camera is null, the sampler
  // will not generate any samples.
  virtual void Init(const Camera* camera) {
    if (camera != NULL) {
      image_.reset(new Image(camera->resolution_x(), camera->resolution_y()));
    }
  }

  // Returns the next sample to be traced or NULL if there are none left. The
  // caller takes ownership of the returned sample.
  virtual Sample* NextSample() = 0;

  // Stores the color of the sample in the image.
  virtual void AcceptSample(const Sample& sample) = 0;

  const Image& image() const { return *image_; }

 protected:
  // TODO(dinow): Possibly make this a private attribute of each sampler.
  std::unique_ptr<Image> image_;
};

#endif  /* SAMPLER_H_ */
