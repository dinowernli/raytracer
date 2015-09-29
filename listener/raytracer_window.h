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
 * A glut-powered user interface for the ray tracer.
 * Author: "Dino Wernli"
 */

#ifndef RAYTRACER_WINDOW_H_
#define RAYTRACER_WINDOW_H_

#include <cstddef>
#include <memory>

#include "renderer/updatable.h"
#include "util/no_copy_assign.h"

class Image;

class RaytracerWindow : public Updatable {
 public:
  RaytracerWindow(int* argc, char **argv);
  virtual ~RaytracerWindow();
  NO_COPY_ASSIGN(RaytracerWindow);

  static const char kWindowTitle[];

  virtual void Started(const Sampler& sampler);
  virtual void Updated(const Sampler& sampler);
  virtual void Ended(const Sampler& sampler);

  void MainLoop();

 private:
  // Needed to register c-style callback for glut.
  static RaytracerWindow* callback_instance_;
  static void display_callback() { callback_instance_->Display(); }
  static void idle_callback() { callback_instance_->Idle(); }

  static const size_t kSleepTimeMilli;

  void Display();
  void Idle();

  // Determines where to take the pixels from when drawing. Could either be an
  // own image or one which lives in the rederer.
  const Image* pixel_source_;

  // When the renderer finishes, this stores a copy of the image.
  std::unique_ptr<Image> image_;

  bool needs_redraw_;
};

#endif  /* RAYTRACER_WINDOW_H_ */
