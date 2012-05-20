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
}

RaytracerWindow::~RaytracerWindow() {
}

void RaytracerWindow::Started(const Sampler& sampler) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutInitWindowSize(sampler.image().SizeX(), sampler.image().SizeY());
  glutCreateWindow(kWindowTitle);

  // TODO(dinow): Find a cleaner way to handle this. Currently, this
  // implementation relies on the fact that the Image below stays valid.
  image_ = NULL;

  needs_redraw_ = true;
  glutDisplayFunc(display_callback);
  glutIdleFunc(idle_callback);

  MainLoop();
}

void RaytracerWindow::Updated(const Sampler& sampler) {
  needs_redraw_ = true;
}

void RaytracerWindow::Ended(const Sampler& sampler) {
  glutDisplayFunc(NULL);
  glutIdleFunc(NULL);
}

void RaytracerWindow::MainLoop() {
  glutMainLoop();
}

void RaytracerWindow::Idle() {
  if (needs_redraw_) {
    glutPostRedisplay();
  }
}

void RaytracerWindow::Display() {
  needs_redraw_ = false;
  const size_t width = image_->SizeX();
  const size_t height = image_->SizeY();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
  //glRasterPos2i(-1,-1);
  glDrawPixels(width, height, GL_RGB, GL_FLOAT, image_->RawData());
  glutSwapBuffers();
}

const char RaytracerWindow::kWindowTitle[] = "Raytracer";
