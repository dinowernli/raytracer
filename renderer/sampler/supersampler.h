/*
 * Produces multiple sub-samples for a given sample.
 * TODO(dinow): Unify the "sampler" interface such that this class can also
 * implement it. Buils some sort of sampling pipeline.
 *
 * Author: "Dino Wernli"
 */

#ifndef SUPERSAMPLER_H_
#define SUPERSAMPLER_H_

#include <cstddef>
#include <vector>

#include "util/no_copy_assign.h"
#include "util/numeric.h"

class Sample;

class Supersampler {
 public:
  Supersampler(size_t rays_per_pixel);
  virtual ~Supersampler();
  NO_COPY_ASSIGN(Supersampler);

  // Populates the samples in target with subsamples for the pixel at base.
  // If the size of target is not rays_per_pixel(), target is resized.
  void GenerateSubsamples(const Sample& base,
                             std::vector<Sample>* target) const;

  size_t rays_per_pixel() const { return rays_per_pixel_; }

 private:
  size_t rays_per_pixel_;

  // Stores the largest x such that x*x <= rays_per_pixel_. This represents the
  // x*x sampling square within the pixel.
  size_t root_num_subpixels_;

  // Stores the size of each of the root_num_subpixels_^2 subpixels.
  Scalar subpixel_size_;
};

#endif  /* SUPERSAMPLER_H_ */
