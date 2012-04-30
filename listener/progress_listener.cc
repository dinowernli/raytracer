/*
 * Author: Dino Wernli
 */

#include "progress_listener.h"

#include <iostream>

#include "renderer/sampler/sampler.h"

ProgressListener::ProgressListener() : last_dumped_progess_(-1) {
}

ProgressListener::~ProgressListener() {
}

void ProgressListener::Update(const Sampler& sampler) {
  int digit = 10 * sampler.Progress();
  if (digit > last_dumped_progess_) {
    std::cout << "Progress: " << 10 * digit << "%" << std::endl;
    last_dumped_progess_ = digit;
  }
}
