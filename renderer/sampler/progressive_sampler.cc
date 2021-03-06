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
 * Author: Dino Wernli
 */

#include "progressive_sampler.h"

#include <cmath>

#include "renderer/sampler/sample.h"

ProgressiveSampler::ProgressiveSampler(bool thread_safe)
    : Sampler(thread_safe) {
}

ProgressiveSampler::~ProgressiveSampler() {
}

void ProgressiveSampler::Init(size_t resolution_x, size_t resolution_y) {
  Sampler::Init(resolution_x, resolution_y);
  current_x_ = 0;
  current_y_ = 0;

  // Set the initial size to the next power of 2.
  size_t max_size = std::max(height(), width());
  current_size_ = pow(2, ceil(log2(max_size)));

  // Initialize the priority map to lower priorities than will ever occur.
  priority_map_ = std::vector<std::vector<int>>(width(),
      std::vector<int>(height(), -(current_size_ * current_size_ + 1)));
}

bool ProgressiveSampler::InternalNextSample(Sample* sample) {
  bool result = false;
  if (current_size_ > 0) {
    sample->set_x(current_x_);
    sample->set_y(current_y_);
    sample->set_size_x(current_size_);
    sample->set_size_y(current_size_);
    DVLOG(3) << "Returning sample: " << *sample;
    result = true;
  }

  // Advance to next sample.
  do {
    current_x_ += current_size_;
    if (current_x_ >= width()) {
      current_x_ = 0;
      current_y_ += current_size_;
      if (current_y_ >= height()) {
        current_y_ = 0;
        current_size_ /= 2;
      }
    }
  } while(current_size_ > 0 && current_x_ % (2 * current_size_) == 0
                            && current_y_ % (2 * current_size_) == 0);
  return result;
}

size_t ProgressiveSampler::NextJob(std::vector<Sample>* samples) {
  std::unique_lock<std::mutex> guard;
  if (IsThreadSafe()) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }

  size_t jobs_added = 0;
  while(jobs_added < kJobSize) {
    Sample& sample = samples->at(jobs_added);
    if (InternalNextSample(&sample)) {
      ++jobs_added;
    } else {
      break;
    }
  }
  return jobs_added;
}

void ProgressiveSampler::AcceptJob(const std::vector<Sample>& samples,
                                   size_t n) {
  std::unique_lock<std::mutex> guard;
  if (IsThreadSafe()) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }

  for (size_t i = 0; i < n; ++i) {
    const Sample& sample = samples[i];
    for (size_t dx = 0; dx < sample.size_x(); ++dx) {
      size_t x = sample.x() + dx;
      if (x >= width()) {
        break;
      }
      for (size_t dy = 0; dy < sample.size_y(); ++dy) {
        size_t y = sample.y() + dy;
        if (y >= height()) {
          break;
        }

        int priority = -1 * (int)(sample.size_x() * sample.size_y());
        if (priority_map_[x][y] < priority) {
          image_->PutPixel(sample.color(), x, y);
          priority_map_[x][y] = priority;
        }
      }
    }
  }
  IncrementAccepted(n);
}

// static
// TODO(dinow): Figure out a decent job size by benchmarking.
const size_t ProgressiveSampler::kJobSize = 8;
