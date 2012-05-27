/*
 * Author: "Dino Wernli"
 */

#include "supersampler.h"

#include <cmath>
#include <ctime>
#include <glog/logging.h>
#include <random>

#include "renderer/sampler/sample.h"

// Returns a uniformly distributed random sample in [-boundary, boundary].
static inline Scalar RandomJitter(Scalar boundary) {
  Scalar result = std::rand();
  result = ((result / RAND_MAX) - 0.5) * 2 * boundary;
  return result;
}

Supersampler::Supersampler(size_t rays_per_pixel)
    : rays_per_pixel_(rays_per_pixel) {
  root_num_subpixels_ = sqrt(rays_per_pixel_);
  subpixel_size_ = Scalar(1) / root_num_subpixels_;

  // Initialize the pseudorandom generator used for jittering samples.
  // TODO(dinow): Replace this by an object (as opposed to global).
  std::srand(time(0));
}

Supersampler::~Supersampler() {
}

void Supersampler::GenerateSubsamples(const Sample& base,
    std::vector<Sample>* target) const {
  if (target->size() != rays_per_pixel()) {
    target->resize(rays_per_pixel());
  }

  // Generate the evenly distributed samples.
  Scalar half_pixel = subpixel_size_ / 2;
  size_t linear_index = 0;

  for (size_t i = 0; i < root_num_subpixels_; ++i) {
    Scalar base_x_offset = half_pixel + i * subpixel_size_;
    for (size_t j = 0; j < root_num_subpixels_; ++j) {
      Scalar base_y_offset = half_pixel + j * subpixel_size_;

      // Set the offset to go through the jittered center of the subpixel.
      Sample* current = &target->at(linear_index++);
      *current = base;
      Scalar jitter_x = rays_per_pixel_ == 1 ? 0 : RandomJitter(half_pixel);
      Scalar jitter_y = rays_per_pixel_ == 1 ? 0 : RandomJitter(half_pixel);
      current->set_offset_x(base_x_offset + jitter_x);
      current->set_offset_y(base_y_offset + jitter_y);

      DVLOG(4) << "Generating subsample " << *current << " at index "
               << linear_index - 1;
    }
  }

  DVLOG(4) << "Filling rest with random samples";

  // Fill the rest with completely random samples in the original pixel.
  for(size_t i = linear_index; i < rays_per_pixel(); ++i) {
    Sample* current = &target->at(i);
    *current = base;
    current->set_offset_x(RandomJitter(0.5) + 0.5);
    current->set_offset_y(RandomJitter(0.5) + 0.5);

    DVLOG(4) << "Generating subsample " << *current << " at index "
             << i;
  }
}
