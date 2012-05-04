/*
 * Consists of a point and a normal in 3D space. Used as a basic building block
 * for meshes, triangles and other primitives.
 *
 * TODO(dinow): Add a mesh pointer to these vertices (or at least a boolean
 * indicating whether a vertex is part of a mesh). Then, triangles can check the
 * boolean only delete the pointer is they own the vertex.
 *
 * Author: Dino Wernli
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "util/point3.h"
#include "util/vector3.h"

class Vertex {
 public:
  Vertex(const Point3& point, const Vector3& normal)
      : point_(point), normal_(normal){
  }
  ~Vertex() {
  }

  const Point3& point() const { return point_; }
  void set_point(const Point3& point) { point_ = point; }

  const Vector3& normal() const { return normal_; }
  void set_normal(const Vector3& normal) { normal_ = normal; }

 private:
  Point3 point_;
  Vector3 normal_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Vertex& v)
{
  return os << "(vertex: " << v.point() << ", " << v.normal() << ")";
}


#endif  /* VERTEX_H_ */
