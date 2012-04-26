/*
 * A sampler which simply samples the pixels in linear order.
 * Author: Dino Wernli
 */

#ifndef SCANLINE_SAMPLER_H_
#define SCANLINE_SAMPLER_H_

#include "util/no_copy_assign.h"

#include "renderer/sampler/sampler.h"

class Camera;
class Sample;

class ScanlineSampler : public Sampler {
 public:
  ScanlineSampler();
  virtual ~ScanlineSampler();
  NO_COPY_ASSIGN(ScanlineSampler);

  virtual void Init(const Camera* camera);
  virtual Sample* NextSample();

 private:
  // Iteration variables for the scan line.
  size_t current_x_;
  size_t current_y_;

  // Size of the image.
  size_t width_;
  size_t height_;
};

#endif  /* SCANLINE_SAMPLER_H_ */
