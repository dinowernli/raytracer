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
 * A sampler which progressively fills out the picture.
 * Author: Dino Wernli
 */

#ifndef PROGRESSIVE_SAMPLER_H_
#define PROGRESSIVE_SAMPLER_H_

#include <vector>

#include "renderer/sampler/sampler.h"
#include "util/no_copy_assign.h"

class ProgressiveSampler : public Sampler {
 public:
  ProgressiveSampler(bool thread_safe);
  virtual ~ProgressiveSampler();
  NO_COPY_ASSIGN(ProgressiveSampler);

  // Not made thread safe, expected to be called only once.
  virtual void Init(size_t resolution_x, size_t resolution_y);

  virtual size_t MaxJobSize() const { return kJobSize; }
  virtual size_t NextJob(std::vector<Sample>* samples);
  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n);

 private:
  // Helper method which fetches the next sample. Returns true if the new sample
  // was successfully stored in sample, and false if there are no samples left.
  bool InternalNextSample(Sample* sample);

  // Iteration variables.
  size_t current_x_;
  size_t current_y_;
  size_t current_size_;

  // Stores a priority for each pixel in order to be able to receive colored
  // samples back out of order.
  std::vector<std::vector<int>> priority_map_;

  // Lock used for synchronizing access to the critical methods.
  std::mutex lock_;

  // TODO(dinow): Increase this over time.
  static const size_t kJobSize;
};

#endif  /* PROGRESSIVE_SAMPLER_H_ */
