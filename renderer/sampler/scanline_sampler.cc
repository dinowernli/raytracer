/*
 * Author: Dino Wernli
 */

#include "scanline_sampler.h"

#include "renderer/sampler/sample.h"
#include "scene/camera.h"

ScanlineSampler::ScanlineSampler(bool thread_safe) : Sampler(thread_safe) {
}

ScanlineSampler::~ScanlineSampler() {
}

void ScanlineSampler::Init(const Camera* camera) {
  Sampler::Init(camera);
  current_x_ = 0;
  current_y_ = 0;
}

bool ScanlineSampler::InternalNextSample(Sample* sample) {
  if (current_x_ >= width()) {
    current_x_ = 0;
    ++current_y_;
  }

  if (current_y_ >= height()) {
    return false;
  }

  sample->set_color(Color3(0, 0, 0));
  sample->set_x(current_x_++);
  sample->set_y(current_y_);
  return true;
}

size_t ScanlineSampler::NextJob(std::vector<Sample>* samples) {
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

void ScanlineSampler::AcceptJob(const std::vector<Sample>& samples, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    // The point (0, 0) in sample space is at the bottom left corner. However, in
    // image space, (0, 0) represents the top left corner. Therefore, we must flip
    // the y-coordinate when writing the color.
    const Sample& sample = samples[i];
    image_->PutPixel(sample.color(), sample.x(), sample.y());
  }

  // Only lock here because the code above is thread-safe.
  std::unique_lock<std::mutex> guard;
  if (IsThreadSafe()) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }
  IncrementAccepted(n);
}

// static
// TODO(dinow): Figure out a decent job size by benchmarking.
const size_t ScanlineSampler::kJobSize = 8;
