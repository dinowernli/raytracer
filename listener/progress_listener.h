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
 * An update listener which tracks progress and logs it.
 * Author: Dino Wernli
 */

#ifndef PROGRESS_LISTENER_H_
#define PROGRESS_LISTENER_H_

#include "renderer/updatable.h"

class Sampler;

class ProgressListener : public Updatable {
 public:
  ProgressListener();
  virtual ~ProgressListener();

  virtual void Started(const Sampler& sampler);
  virtual void Updated(const Sampler& sampler);
  virtual void Ended(const Sampler& sampler);

 private:
  void LogProgress(const Sampler& sampler);

  double last_dumped_progess_;
};

#endif  /* PROGRESS_LISTENER_H_ */
