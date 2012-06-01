/*
 * Author: "Dino Wernli"
 */

#include "supersampler.h"

#include <cmath>
#include <glog/logging.h>

#include "renderer/sampler/sample.h"

Supersampler::Supersampler(size_t root_rays_per_pixel)
    : root_rays_per_pixel_(root_rays_per_pixel) {
  rays_per_pixel_ = root_rays_per_pixel_ * root_rays_per_pixel_;
  subpixel_size_ = Scalar(1) / root_rays_per_pixel_;
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
  size_t index = 0;

  for (size_t i = 0; i < root_rays_per_pixel_; ++i) {
    Scalar base_x_offset = half_pixel + i * subpixel_size_;
    for (size_t j = 0; j < root_rays_per_pixel_; ++j) {
      Scalar base_y_offset = half_pixel + j * subpixel_size_;

      // Disable jittering if there are only few rays.
      Scalar jitter_x = WillJitter() ? random_.Get(half_pixel) : 0;
      Scalar jitter_y = WillJitter() ? random_.Get(half_pixel) : 0;

      // Set the offset to go through the jittered center of the subpixel.
      Sample* sample = &target->at(index++);
      *sample = base;
      sample->set_offset_x(base_x_offset + jitter_x);
      sample->set_offset_y(base_y_offset + jitter_y);
      DVLOG(4) << "Generating subsample " << *sample << " at index " << index-1;
    }
  }
}

// static
const size_t Supersampler::kJitterThreshold = 4;

// static
Random Supersampler::random_;
