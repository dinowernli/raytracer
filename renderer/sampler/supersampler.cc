/*
 * Author: "Dino Wernli"
 */

#include "supersampler.h"

#include <cmath>
#include <ctime>
#include <glog/logging.h>

#include "renderer/sampler/sample.h"

Supersampler::Supersampler(size_t rays_per_pixel)
    : rays_per_pixel_(rays_per_pixel), distribution_(-1, 1) {
  root_num_subpixels_ = sqrt(rays_per_pixel_);
  subpixel_size_ = Scalar(1) / root_num_subpixels_;
  std::random_device device;
  random_engine_.seed(device());
}

Supersampler::~Supersampler() {
}

void Supersampler::GenerateSubsamples(const Sample& base,
    std::vector<Sample>* target) {
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

      // Disable jittering if there are only few rays.
      Scalar jitter_x = WillJitter() ? Random(half_pixel) : 0;
      Scalar jitter_y = WillJitter() ? Random(half_pixel) : 0;

      // Set the offset to go through the jittered center of the subpixel.
      Sample* current = &target->at(linear_index++);
      *current = base;
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
    current->set_offset_x(Random(0.5) + 0.5);
    current->set_offset_y(Random(0.5) + 0.5);

    DVLOG(4) << "Generating subsample " << *current << " at index "
             << i;
  }
}

const size_t Supersampler::kJitterThreshold = 4;
