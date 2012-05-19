/*
 * A generic listener which can react to events triggered by the Renderer.
 * Author: Dino Wernli
 */

#ifndef UPDATABLE_H_
#define UPDATABLE_H_

class Sampler;

class Updatable {
 public:
  virtual ~Updatable() { };
  virtual void Started(const Sampler& sampler) {}
  virtual void Updated(const Sampler& sampler) {}
  virtual void Ended(const Sampler& sampler) {}
};

#endif  /* UPDATABLE_H_ */
