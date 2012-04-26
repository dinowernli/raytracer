/*
 * Author: Dino Wernli
 */

#include "scanline_sampler.h"

#include "renderer/sampler/sample.h"
#include "scene/camera.h"

ScanlineSampler::ScanlineSampler() {
}

ScanlineSampler::~ScanlineSampler() {
}

void ScanlineSampler::Init(const Camera* camera) {
  Sampler::Init(camera);
  current_x_ = 0;
  current_y_ = 0;

  if (camera != NULL) {
    width_ = camera->resolution_x();
    height_ = camera->resolution_y();
  } else {
    width_ = 0;
    height_ = 0;
  }
}

Sample* ScanlineSampler::NextSample() {
  if (current_x_ >= width_) {
    current_x_ = 0;
    ++current_y_;
  }

  if (current_y_ >= height_) {
    return NULL;
  }

  // TODO(dinow): Reuse allocated sample instead of creating new ones.
  return new Sample(current_x_++, current_y_);
}

void ScanlineSampler::AcceptSample(const Sample& sample) {
  image_->PutPixel(sample.color(), sample.x(), sample.y());
}
