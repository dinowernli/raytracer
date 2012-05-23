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

void ProgressiveSampler::Init(const Camera* camera) {
  Sampler::Init(camera);
  current_x_ = 0;
  current_y_ = 0;

  first_ = true;

  // Set the initial size to the next power of 2.
  size_t max_size = std::max(height(), width());
  current_size_ = pow(2, ceil(log2(max_size)));

  // Initialize the priority map to lower priorities than will ever occur.
  priority_map_ = std::vector<std::vector<int>>(height(),
      std::vector<int>(width(), -(current_size_ * current_size_ + 1)));
}

bool ProgressiveSampler::InternalNextSample(Sample* sample) {
  if (first_) {
    first_ = false;
    sample->set_x(current_x_);
    sample->set_y(current_y_);
    sample->set_size_x(current_size_);
    sample->set_size_y(current_size_);
    DVLOG(1) << "Handing out sample " << *sample;
    return true;
  }

  // Advance to next sample and if too far, return false.
  current_x_ += current_size_;
  if (current_x_ >= width()) {
    current_x_ = 0;
    current_y_ += current_size_;
    if (current_y_ >= height()) {
      current_y_ = 0;
      current_size_ /= 2;
    }
  }

  if (current_size_ <= 0) {
    DVLOG(1) << "Sample size hit 0, stopping";
    return false;
  }

  size_t last_size = 2 * current_size_;
  if (current_x_ % last_size == 0 && current_y_ % last_size == 0) {
    return InternalNextSample(sample);
  }

  // TODO(dinow): Use the fact that samples support different sizes for x and y.
  sample->set_x(current_x_);
  sample->set_y(current_y_);
  sample->set_size_x(current_size_);
  sample->set_size_y(current_size_);

  DVLOG(1) << "Handing out sample " << *sample;
  return true;
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
