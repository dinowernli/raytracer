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
 * Produces multiple sub-samples for a given sample.
 * TODO(dinow): Unify the "sampler" interface such that this class can also
 * implement it. Buils some sort of sampling pipeline.
 *
 * Author: "Dino Wernli"
 */

#ifndef SUPERSAMPLER_H_
#define SUPERSAMPLER_H_

#include <cstddef>
#include <glog/logging.h>
#include <vector>

#include "util/color3.h"
#include "util/numeric.h"
#include "util/random.h"

class Sample;
class Statistics;

struct Accumulator {
  Accumulator() : num_samples(0), sum(0, 0, 0) {}

  size_t num_samples;
  Color3 sum;

  void Process(const Color3& x) {
    ++num_samples;
    sum += x;
  }

  Color3 Mean() const {
    if (num_samples == 0) {
      return Color3(0, 0, 0);
    } else {
      return (1.0 / num_samples) * sum;
    }
  }
};

class Supersampler {
 public:
  // Sets an initial root of rays per pixel. If the adaptive threshold greater
  // than 0, the supersampler will keep increasing the number of samples until
  // the variance goes below the threshold. Takes no ownership of statistics.
  Supersampler(size_t root_rays_per_pixel = 1, Scalar threshold = -1,
               Statistics* statistics = NULL);
  virtual ~Supersampler();

  // Populates the samples in target with subsamples for the pixel at base.
  // Stores the new samples at the beginning of target and returns how many
  // samples were generated. If target is too small, target is resized.
  size_t GenerateSubsamples(const Sample& base, std::vector<Sample>* target);

  // In adaptive mode this helps the supersampler figure out if more samples are
  // necessary. Will only consider the first n_samples samples in samples.
  void ReportResults(const std::vector<Sample>& samples, size_t n_samples);

  Color3 MeanResults() const { return accum_.Mean(); }

  // Returns whether or not the subsamples get jittered. Jittering is
  // deactivated for low sample number in order to avoid excessive variance.
  bool WillJitter() const { return rays_per_pixel_ >= kJitterThreshold; }

  bool IsAdaptive() const { return threshold_ > 0; }

 private:
  void ComputeCachedValues();
  void UpdateStatistics(size_t num_samples, size_t x, size_t y);

  // A threshold for rays_per_pixel_ below which jittering is disabled in order
  // to prevent large variance.
  static const size_t kJitterThreshold;

  // A threshold for adaptive supersampling. If positive, the supersampler will
  // produce more samples until the variance goes below this threshold.
  Scalar threshold_;
  bool update_below_threshold_;
  Accumulator accum_;

  // Needed to distinguish the first round from subsequent rounds. In the first
  // round, the supersampler does not skip any samples. Also, if adaptive is
  // off, only the first round is generated.
  bool first_round_;

  // Stores the largest x such that x*x <= rays_per_pixel_. This represents the
  // x*x sampling square within the pixel.
  size_t root_rays_per_pixel_;

  // Cached values in order to avoid repeating computations.
  size_t rays_per_pixel_;
  Scalar subpixel_size_;
  Scalar half_subpixel_;

  // Used to keep track of the number of rays casted for each pixel.
  Statistics* statistics_;

  mutable Random random_;
};

#endif  /* SUPERSAMPLER_H_ */
