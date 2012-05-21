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
  image_ = &sampler.image();
  glutReshapeWindow(image_->SizeX(), image_->SizeY());
  needs_redraw_ = true;
}

void RaytracerWindow::Updated(const Sampler& sampler) {
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
  const size_t width = image_->SizeX();
  const size_t height = image_->SizeY();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
  glRasterPos2i(-1,-1);
  glDrawPixels(width, height, GL_RGB, GL_FLOAT, image_->RawData());
  glutSwapBuffers();
}

// static
const char RaytracerWindow::kWindowTitle[] = "Raytracer";

// static
const size_t RaytracerWindow::kSleepTimeMilli = 300;
