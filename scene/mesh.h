/*
 * A mesh of triangles. Stores all vertices of the mesh and knows how to produce
 * light-weight elements.
 * Author: Dino Wernli
 */

#ifndef MESH_H_
#define MESH_H_

#include<memory>
#include<vector>

#include "scene/geometry/vertex.h"

class Element;
class Point3;
class Vector3;

class Mesh {
 public:
  Mesh();
  virtual ~Mesh();

  // Adds a new vertex to the mesh and returns the index of the vertex. The
  // mesh guarantees that the first added vertex will have index 0 and that
  // every subsequent add will increase the vertex index by 1.
  size_t AddVertex(const Point3& point, const Vector3& normal);

  // Declares a triangle using the three passed vertex indices.
  void AddTriangle(size_t v1, size_t v2, size_t v3);

  // Adds light-weight triangles to target. The caller takes ownership of the
  // produced triangles.
  void CreateElements(std::vector<std::unique_ptr<Element>>* target) const;

 private:
  struct TriangleDescriptor {
    TriangleDescriptor(size_t v1, size_t v2, size_t v3) : v1_(v1), v2_(v2),
                                                          v3_(v3) {
    }
    size_t v1_;
    size_t v2_;
    size_t v3_;
  };

  std::vector<std::unique_ptr<Vertex>> vertices_;
  std::vector<TriangleDescriptor> descriptors_;
};

#endif  /* MESH_H_ */
