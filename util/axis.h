/*
 * A type safe implementation of an axis in 3D space. Only the values 'x', 'y'
 * and 'z' are legal.
 * Author: Dino Wernli
 */

#ifndef AXIS_H_
#define AXIS_H_

class Axis {
 public:
  static inline Axis x() { return Axis(0); }
  static inline Axis y() { return Axis(1); }
  static inline Axis z() { return Axis(2); }

  // Maps x to y, y to z and z to x.
  Axis Next() const { return Axis((id_ + 1) % 3); }
  bool operator==(const Axis& other) const { return other.id_ == id_; }

 private:
  typedef unsigned char axis_id_t;
  explicit Axis(axis_id_t id) : id_(id) {}
  axis_id_t id_;
};

#endif  /* AXIS_H_ */
