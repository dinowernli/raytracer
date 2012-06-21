/*
 * A collection of utilities used in various unit tests.
 * Author: Dino Wernli
 */

#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include "util/color3.h"

class TestUtil {
 public:
  static bool ColorsEqual(Color3 c1, Color3 c2) {
    return (c1.r() == c2.r() && c1.g() == c2.g() && c1.b() == c2.b());
  }
};

#endif  /* TEST_UTIL_H_ */
