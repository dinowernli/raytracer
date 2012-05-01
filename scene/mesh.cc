/*
 * Author: Dino Wernli
 */

#include "mesh.h"

#include "scene/element.h"
#include "scene/geometry/mesh_triangle.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

void Mesh::CreateElements(std::vector<Element*>* target) {
  for (auto it = descriptors_.begin(); it != descriptors_.end(); ++it) {
    const Vertex* v1 = vertices_[it->v1_].get();
    const Vertex* v2 = vertices_[it->v2_].get();
    const Vertex* v3 = vertices_[it->v3_].get();
    target->push_back(new MeshTriangle(v1, v2, v3));
  }
}
