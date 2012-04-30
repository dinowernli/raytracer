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

  virtual void Update(const Sampler& sampler);

 private:
  double last_dumped_progess_;
};

#endif  /* PROGRESS_LISTENER_H_ */
