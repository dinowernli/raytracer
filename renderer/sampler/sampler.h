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
 * Represents an objects which is in charge of generating samples for a given
 * camera. The samples can then be traced and passed back to the sampler once
 * they contain a color. The sampler also manages the process of inserting
 * colored samples into an image.
 * Author: Dino Wernli
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <glog/logging.h>
#include <memory>
#include <mutex>

#include "renderer/image.h"

class Image;
class Sample;

class Sampler {
 public:
  Sampler(bool thread_safe) : thread_safe_(thread_safe) {}
  virtual ~Sampler() {};

  // Prepares for generating samples for a given resolution.
  virtual void Init(size_t resolution_x, size_t resolution_y) {
    if (resolution_x == 0 || resolution_y == 0) {
      LOG(WARNING) << "Initializing sampler with 0 resolution.";
    }
    image_.reset(new Image(resolution_x, resolution_y));
    accepted_ = 0;
  }

  // Returns the maximum size of a rendering job provided by NextJob.
  virtual size_t MaxJobSize() const = 0;

  // Fills at most the first MaxJobSize() samples with new samples to be traced.
  // Expects the passed vector to be large enough already. Returns the number of
  // new jobs which were filled into the (beginning of the) vector.
  virtual size_t NextJob(std::vector<Sample>* samples) = 0;

  // Writes the colors of the first n elements of "samples" into the image.
  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n) = 0;

  // Returns whether the sampler is prepared to handle multiple threads calling
  // its methods concurrently.
  virtual bool IsThreadSafe() const { return thread_safe_; }

  const Image& image() const { return *image_; }
  size_t width() const { return image_.get() == NULL ? 0 : image_->SizeX(); }
  size_t height() const { return image_.get() == NULL ? 0 : image_->SizeY(); }

  // Returns the progress of the rendering as value in [0, 1].
  virtual double Progress() const {
    size_t total_size = image().SizeX() * image_->SizeY();
    if (total_size == 0) {
      return 1;
    }
    return double(accepted_) / total_size;
  }

  // Returns true iff all pixels have been written to the image.
  bool IsDone() const { return Progress() == 1.0; }

 protected:
  // Intended for use by children indicating that samples have been returned.
  void IncrementAccepted(size_t samples) { accepted_ += samples; }

  // Protected to allow children to modify it.
  std::unique_ptr<Image> image_;
  std::mutex lock_;

 private:
  size_t accepted_;
  bool thread_safe_;
};

#endif  /* SAMPLER_H_ */
