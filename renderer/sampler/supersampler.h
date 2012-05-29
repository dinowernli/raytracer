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
#include "util/random.h"

class Sample;

class Supersampler {
 public:
  Supersampler(size_t rays_per_pixel);
  virtual ~Supersampler();
  NO_COPY_ASSIGN(Supersampler);

  // Populates the samples in target with subsamples for the pixel at base.
  // If the size of target is not rays_per_pixel(), target is resized.
  void GenerateSubsamples(const Sample& base,
                             std::vector<Sample>* target);

  size_t rays_per_pixel() const { return rays_per_pixel_; }

  // Returns whether or not the subsamples get jittered. Jittering is
  // deativated for low sample number in order to avoid excessive variance.
  bool WillJitter() const { return rays_per_pixel_ >= kJitterThreshold; }

 private:
  // This is static so that getting a random number does not mutate the object.
  static Random random_;

  // A threshold for rays_per_pixel_ below which jittering is disabled in order
  // to prevent large variance.
  static const size_t kJitterThreshold;

  size_t rays_per_pixel_;

  // Stores the largest x such that x*x <= rays_per_pixel_. This represents the
  // x*x sampling square within the pixel.
  size_t root_num_subpixels_;

  // Stores the size of each of the root_num_subpixels_^2 subpixels.
  Scalar subpixel_size_;
};

#endif  /* SUPERSAMPLER_H_ */
