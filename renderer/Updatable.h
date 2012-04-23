/*
 * A generic listener which can react to events triggered by the Renderer.
 * Author: Dino Wernli
 */

#ifndef UPDATABLE_H_
#define UPDATABLE_H_

class Image;

class Updatable {
 public:
  virtual ~Updatable() { };
  virtual void Update(const Image& image) = 0;
};

#endif  /* UPDATABLE_H_ */
