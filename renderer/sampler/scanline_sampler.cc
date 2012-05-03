/*
 * Author: Dino Wernli
 */

#include "scanline_sampler.h"

#include "renderer/sampler/sample.h"
#include "scene/camera.h"

ScanlineSampler::ScanlineSampler(bool thread_safe) : thread_safe_(thread_safe) {
}

ScanlineSampler::~ScanlineSampler() {
}

void ScanlineSampler::Init(const Camera* camera) {
  Sampler::Init(camera);
  current_x_ = 0;
  current_y_ = 0;
  accepted_ = 0;

  // After the init call above, the image is initialized.
  width_ = image().SizeX();
  height_ = image().SizeY();
}

bool ScanlineSampler::InternalNextSample(Sample* sample) {
  if (current_x_ >= width_) {
    current_x_ = 0;
    ++current_y_;
  }

  if (current_y_ >= height_) {
    return false;
  }

  sample->set_color(Color3(0, 0, 0));
  sample->set_x(current_x_++);
  sample->set_y(current_y_);
  return true;
}

size_t ScanlineSampler::NextJob(std::vector<Sample>* samples) {
  std::unique_lock<std::mutex> guard;
  if (thread_safe_) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }

  size_t jobs_added = 0;
  const size_t max_size = MaxJobSize();
  while(jobs_added < max_size) {
    Sample& sample = samples->at(jobs_added);
    if (InternalNextSample(&sample)) {
      ++jobs_added;
    } else {
      break;
    }
  }
  return jobs_added;
}

bool ScanlineSampler::NextSample(Sample* sample) {
  std::unique_lock<std::mutex> guard;
  if (thread_safe_) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }
  return InternalNextSample(sample);
}

void ScanlineSampler::InternalAcceptSample(const Sample& sample) {
  // The point (0, 0) in sample space is at the bottom left corner. However, in
  // image space, (0, 0) represents the top left corner. Therefore, we must flip
  // the y-coordinate when writing the color.
  const size_t image_y = image_->SizeY() - sample.y() - 1;
  image_->PutPixel(sample.color(), sample.x(), image_y);
  ++accepted_;
}

void ScanlineSampler::AcceptSample(const Sample& sample) {
  std::unique_lock<std::mutex> guard;
  if (thread_safe_) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }
  InternalAcceptSample(sample);
}

void ScanlineSampler::AcceptJob(const std::vector<Sample>& samples, size_t n) {
  std::unique_lock<std::mutex> guard;
  if (thread_safe_) {
    guard = std::move(std::unique_lock<std::mutex>(lock_));
  }

  for (size_t i = 0; i < n; ++i) {
    InternalAcceptSample(samples[i]);
  }
}

double ScanlineSampler::Progress() const {
  size_t total_size = image_->SizeX() * image_->SizeY();
  if (total_size == 0) {
    return 1;
  }
  return double(accepted_) / (image_->SizeX() * image_->SizeY());
}

// static
const size_t ScanlineSampler::kJobSize = 1;
