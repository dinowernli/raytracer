/*
 * Represents an objects which is in charge of generating samples for a given
 * camera. The samples can then be traced and passed back to the sampler once
 * they contain a color. The sampler also manages the process of inserting
 * colored samples into an image.
 * Author: Dino Wernli
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <glog/logging.h>
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
    } else {
      LOG(WARNING) << "Initializing sampler with NULL camera.";
      image_.reset(new Image(0, 0));
    }
  }

  // Returns whether there is a next sample to be traced. If it returns true,
  // then the next sample is stored in 'sample', the color of the sample is set
  // to black.
  virtual bool NextSample(Sample* sample) = 0;

  // Fills at most the first MaxJobSize() samples with new samples to be traced.
  // Expects the passed vector to be large enough already. Returns the number of
  // new jobs which were filled into the (beginning of the) vector.
  virtual size_t NextJob(std::vector<Sample>* samples) = 0;

  // Indicated the maximum size of a rendering job which is fetched by the
  // NextJob method.
  virtual size_t MaxJobSize() const = 0;

  // Stores the color of the sample in the image.
  virtual void AcceptSample(const Sample& sample) = 0;

  // Writes the colors of the first n elements of "samples" into the image.
  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n) = 0;

  // Returns whether the sampler is prepared to handle multiple threads calling
  // its methods concurrently.
  virtual bool IsThreadSafe() const = 0;

  const Image& image() const { return *image_; }

  // Returns the progress of the rendering as value in [0, 1].
  virtual double Progress() const = 0;

  // Returns true iff all pixels have been written to the image.
  bool IsDone() const { return Progress() == 1.0; }

 protected:
  // TODO(dinow): Possibly make this a private attribute of each sampler.
  std::unique_ptr<Image> image_;
};

#endif  /* SAMPLER_H_ */
