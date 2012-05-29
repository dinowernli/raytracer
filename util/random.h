/*
 * A utility class which provides uniformly distributed values.
 * Author: Dino Wernli
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

class Random {
 public:
  Random() {
    std::random_device device;
    random_engine_.seed(device());
    distribution_ = std::uniform_real_distribution<Scalar>(0, 1);
  }

  // Returns a uniform random number in [-boundary, boundary].
  Scalar Get(Scalar boundary) {
    return Get(-boundary, boundary);
  }

  // Returns a uniform random number in [lower, upper].
  Scalar Get(Scalar lower, Scalar upper) {
    return distribution_(random_engine_) * (upper - lower) + lower;
  }

 private:
  std::mt19937 random_engine_;
  std::uniform_real_distribution<Scalar> distribution_;
};

#endif  /* RANDOM_H_ */
