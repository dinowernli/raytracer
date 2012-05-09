/*
 * Consists of a point and a normal in 3D space. Used as a building block of
 * triangles.
 *
 * Author: Dino Wernli
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "util/point3.h"
#include "util/vector3.h"

class Vertex {
 public:
  // Copies the passed point and normal and allocated copies.
  Vertex(const Point3& point, const Vector3& normal)
      : point_(new Point3(point)), normal_(new Vector3(normal)),
        owns_data_(true) {
  }

  // Takes no ownership of the passed pointers and copies them. The underlying
  // data is referenced (if it changes, so will this vertex).
  Vertex(const Point3* point, const Vector3* normal)
      : point_(point), normal_(normal), owns_data_(false) {
  }

  ~Vertex() {
    if (owns_data_) {
      delete point_;
      delete normal_;
    }
  }

  const Point3& point() const { return *point_; }
  const Vector3& normal() const { return *normal_; }

 private:
  const Point3* point_;
  const Vector3* normal_;

  // This flag is false iff the data was allocated in a mesh and the vertex is
  // not expected to clean it up.
  bool owns_data_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Vertex& v)
{
  return os << "(vertex: " << v.point() << ", " << v.normal() << ")";
}

#endif  /* VERTEX_H_ */
