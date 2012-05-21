/*
 * Author: Dino Wernli
 */

#include "progressive_sampler.h"

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
  current_size_x_ = 0;
  current_size_y_ = 0;
}

bool ProgressiveSampler::InternalNextSample(Sample* sample) {
  // TODO(dinow): implement
  return false;
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
  // TODO(dinow): implement
}

// static
// TODO(dinow): Figure out a decent job size by benchmarking.
const size_t ProgressiveSampler::kJobSize = 8;
