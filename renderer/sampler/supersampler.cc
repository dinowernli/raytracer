/*
 * Author: "Dino Wernli"
 */

#include "supersampler.h"

#include <cmath>
#include <glog/logging.h>

#include "renderer/sampler/sample.h"

Supersampler::Supersampler(size_t root_rays_per_pixel, Scalar threshold) {
  root_rays_per_pixel_ = root_rays_per_pixel;
  threshold_ = threshold;
  first_round_ = true;
  update_below_threshold_ = false;
  ComputeCachedValues();
}

Supersampler::~Supersampler() {
}

void Supersampler::ReportResults(const std::vector<Sample>& samples,
                                 size_t n_samples) {
  Color3 old_mean = tracker_.Mean();
  for (size_t i = 0; i < n_samples; ++i) {
    tracker_.Process(samples[i].color());
  }

  Color3 diff = tracker_.Mean() - old_mean;
  Scalar avg_update = std::abs((diff.r() + diff.g() + diff.b()) / 3.0);
  DVLOG(1) << "Average update: " << avg_update;
  if (avg_update < threshold_) {
    DVLOG(1) << "Flipping supersampling flag.";
    update_below_threshold_ = true;
  }
}

size_t Supersampler::GenerateSubsamples(const Sample& base,
                                        std::vector<Sample>* target) {
  if (IsAdaptive()) {
    if (!first_round_) {
      if(update_below_threshold_) {
        // Pixel value computed closely enough. Abort.
        DVLOG(1) << "Generated " << rays_per_pixel_ << " samples for pixel ["
                 << base.x() << ", " << base.y() << "]";
        return 0;
      } else {
        // Move on to next level, multiply by 2 to reuse old samples.
        // TODO(dinow): This is not entirely correct because jittering size
        // and half_pixel changes... Somehow fix this.

        //root_rays_per_pixel_ = 2*root_rays_per_pixel_ + 1;
        ++root_rays_per_pixel_;
        DVLOG(1) << "Increasing sample root to " << root_rays_per_pixel_;
        ComputeCachedValues();
      }
    }
  } else {
    if (!first_round_) {
      // In the non-adaptive case, just generate the grid once.
      return 0;
    }
  }
  first_round_ = false;

  if (target->size() < rays_per_pixel_) {
    target->resize(rays_per_pixel_);
  }

  // Generate the evenly distributed samples.
  size_t index = 0;
  for (size_t i = 0; i < root_rays_per_pixel_; ++i) {
    Scalar base_x_offset = half_subpixel_ + i * subpixel_size_;
    for (size_t j = 0; j < root_rays_per_pixel_; ++j) {
      if (!first_round_ && IsAdaptive() && (i % 2 == 1) && (j % 2 == 1)) {
        // Computed in previous round of adaptive sampling. Skip.
        continue;
      }

      Scalar base_y_offset = half_subpixel_ + j * subpixel_size_;

      // Disable jittering if there are only few rays.
      Scalar jitter_x = WillJitter() ? random_.Get(half_subpixel_) : 0;
      Scalar jitter_y = WillJitter() ? random_.Get(half_subpixel_) : 0;

      // Set the offset to go through the jittered center of the subpixel.
      Sample* sample = &target->at(index++);
      *sample = base;
      sample->set_offset_x(base_x_offset + jitter_x);
      sample->set_offset_y(base_y_offset + jitter_y);
      DVLOG(4) << "Generating subsample " << *sample << " at index " << index-1;
    }
  }
  return index;
}

void Supersampler::ComputeCachedValues() {
  rays_per_pixel_ = root_rays_per_pixel_ * root_rays_per_pixel_;
  subpixel_size_ = Scalar(1) / root_rays_per_pixel_;
  half_subpixel_ = subpixel_size_ * 0.5;
}

// static
const size_t Supersampler::kJitterThreshold = 4;
