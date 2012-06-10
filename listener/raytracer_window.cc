/*
 * Author: "Dino Wernli"
 */

#include "raytracer_window.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <glog/logging.h>

#include "renderer/image.h"
#include "renderer/sampler/sampler.h"

RaytracerWindow* RaytracerWindow::callback_instance_ = NULL;

RaytracerWindow::RaytracerWindow(int* argc, char **argv) {
  CHECK(callback_instance_ == NULL) << "Detected duplicate glut window";
  callback_instance_ = this;

  pixel_source_ = NULL;
  image_.reset(NULL);
  needs_redraw_ = false;

  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutCreateWindow(kWindowTitle);

  glutDisplayFunc(display_callback);
  glutIdleFunc(idle_callback);
}

RaytracerWindow::~RaytracerWindow() {
}

void RaytracerWindow::Started(const Sampler& sampler) {
  // The image guarantees to always stay valid.
  pixel_source_ = &sampler.image();
  image_.reset(NULL);

  if (pixel_source_->SizeX() == 0 || pixel_source_->SizeY() == 0) {
    return;
  }
  glutReshapeWindow(pixel_source_->SizeX(), pixel_source_->SizeY());
  needs_redraw_ = true;
}

void RaytracerWindow::Updated(const Sampler& sampler) {
  needs_redraw_ = true;
}

void RaytracerWindow::Ended(const Sampler& sampler) {
  image_.reset(new Image(sampler.image()));
  pixel_source_ = image_.get();
  needs_redraw_ = true;
}

void RaytracerWindow::MainLoop() {
  glutMainLoop();
}

void RaytracerWindow::Idle() {
  if (needs_redraw_) {
    needs_redraw_ = false;
    glutPostRedisplay();
  }
  usleep(kSleepTimeMilli * MILLI_TO_MICRO);
}

void RaytracerWindow::Display() {
  if (pixel_source_ == NULL) {
    // This call happened before the first call to Started(), so there is no
    // image. This is possible because glutPostRedisplay() could be called
    // outside this class.
    return;
  }

  const size_t width = pixel_source_->SizeX();
  const size_t height = pixel_source_->SizeY();

  if (width == 0 || height == 0) {
    return;
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
  glRasterPos2i(-1,-1);
  glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixel_source_->RawData());
  glutSwapBuffers();
}

// static
const char RaytracerWindow::kWindowTitle[] = "Raytracer";

// static
const size_t RaytracerWindow::kSleepTimeMilli = 300;
