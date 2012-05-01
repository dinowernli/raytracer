/*
 * Author: Dino Wernli
 */

#ifndef MESH_TRIANGLE_H_
#define MESH_TRIANGLE_H_

#include "scene/geometry/triangle.h"
#include "util/no_copy_assign.h"

class MeshTriangle : public Triangle {
 public:
  MeshTriangle(const Vertex* v1, const Vertex* v2, const Vertex* v3,
               const Material* material = NULL)
      : Triangle(v1, v2, v3, material) {
  }

  virtual ~MeshTriangle() {
    // Note(dinow): Do not delete the vertices since they belong to the mesh.
  }
};

#endif  /* MESH_TRIANGLE_H_ */
