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

bool ScanlineSampler::NextSample(Sample* sample) {
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

void ScanlineSampler::AcceptSample(const Sample& sample) {
  // The point (0, 0) in sample space is at the bottom left corner. However, in
  // image space, (0, 0) represents the top left corner. Therefore, we must flip
  // the y-coordinate when writing the color.
  const size_t image_y = image_->SizeY() - sample.y() - 1;
  image_->PutPixel(sample.color(), sample.x(), image_y);
}
