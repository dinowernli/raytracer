/*
 * A type safe implementation of an axis in 3D space. Only the values 'x', 'y'
 * and 'z' are legal.
 * Author: Dino Wernli
 */

#ifndef AXIS_H_
#define AXIS_H_

#include <cstddef>

class Axis {
 public:
  // Build an Axis from the rest of "id" modulo 3.
  explicit Axis(size_t id) : id_(id % 3) {}

  static inline Axis x() { return Axis(0); }
  static inline Axis y() { return Axis(1); }
  static inline Axis z() { return Axis(2); }

  // Maps x to y, y to z and z to x.
  Axis Next() const { return Axis(id_ + 1); }
  bool operator==(const Axis& other) const { return other.id_ == id_; }

 private:
  size_t id_;
};

#endif  /* AXIS_H_ */
