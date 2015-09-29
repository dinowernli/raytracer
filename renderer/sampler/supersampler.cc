// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/*
 * Author: "Dino Wernli"
 */

#include "supersampler.h"

#include <cmath>
#include <glog/logging.h>

#include "renderer/sampler/sample.h"
#include "renderer/statistics.h"

Supersampler::Supersampler(size_t root_rays_per_pixel, Scalar threshold,
                           Statistics* statistics) {
  root_rays_per_pixel_ = root_rays_per_pixel;
  threshold_ = threshold;
  first_round_ = true;
  update_below_threshold_ = false;
  statistics_ = statistics;

  if (IsAdaptive() && root_rays_per_pixel <= 3) {
    LOG(WARNING) << "Too few samples per pixel. Setting root_rays to 4 to avoid"
                 << "artifacts from adaptive supersampling";
    root_rays_per_pixel_ = 4;
  }

  ComputeCachedValues();
}

Supersampler::~Supersampler() {
}

void Supersampler::ReportResults(const std::vector<Sample>& samples,
                                 size_t n_samples) {
  Color3 old_mean = accum_.Mean();
  for (size_t i = 0; i < n_samples; ++i) {
    accum_.Process(samples[i].color());
  }

  Color3 diff = accum_.Mean() - old_mean;
  Scalar avg_update = std::abs((diff.r() + diff.g() + diff.b()) / 3.0);
  DVLOG(3) << "Average update: " << avg_update;
  if (avg_update < threshold_) {
    DVLOG(3) << "Flipping supersampling flag.";
    update_below_threshold_ = true;
  }
}

void Supersampler::UpdateStatistics(size_t num_samples, size_t x, size_t y) {
  if (statistics_ != NULL && statistics_->sampling_heatmap() != NULL) {
    Color3 heat(num_samples, num_samples, num_samples);
    statistics_->sampling_heatmap()->PutPixel(heat, x, y);
  }
}

size_t Supersampler::GenerateSubsamples(const Sample& base,
                                        std::vector<Sample>* target) {
  size_t samples = accum_.num_samples;
  if (IsAdaptive()) {
    if (!first_round_) {
      if(update_below_threshold_) {
        // Pixel value computed closely enough. Abort.
        DVLOG(3) << "Generated " << samples << " samples for pixel ["
                 << base.x() << ", " << base.y() << "]";
        UpdateStatistics(samples, base.x(), base.y());
        return 0;
      } else {
        // TODO(dinow): This comes closer to actual reuse, but grows too fast.
        //root_rays_per_pixel_ = 2*root_rays_per_pixel_ + 1;
        ++root_rays_per_pixel_;
        DVLOG(1) << "Increasing sample root to " << root_rays_per_pixel_;
        if (root_rays_per_pixel_ > 100) {
          LOG(WARNING) << "Root rays per pixel exceeding 100 (10'000 rays)";
        }
        ComputeCachedValues();
      }
    }
  } else {
    if (!first_round_) {
      // In the non-adaptive case, just generate the grid once.
      UpdateStatistics(samples, base.x(), base.y());
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
        // Sample very close to a previously computed one, Skip.
        // TODO(dinow): Somehow skip more cleverly.
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
