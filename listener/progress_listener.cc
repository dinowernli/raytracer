/*
 * Author: Dino Wernli
 */

#include "progress_listener.h"

#include <glog/logging.h>

#include "renderer/sampler/sampler.h"

ProgressListener::ProgressListener() : last_dumped_progess_(-1) {
}

ProgressListener::~ProgressListener() {
}

void ProgressListener::Update(const Sampler& sampler) {
  int digit = 10 * sampler.Progress();
  if (digit > last_dumped_progess_) {
    LOG(INFO) << "Progress: " << 10 * digit << "%";
    last_dumped_progess_ = digit;
  }
}
