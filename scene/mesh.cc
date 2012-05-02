/*
 * Author: Dino Wernli
 */

#include "mesh.h"

#include <glog/logging.h>

#include "scene/element.h"
#include "scene/geometry/triangle.h"
#include "util/point3.h"
#include "util/vector3.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

size_t Mesh::AddVertex(const Point3& point, const Vector3& normal) {
  vertices_.push_back(std::unique_ptr<Vertex>(new Vertex(point, normal)));
  return vertices_.size() - 1;
}

void Mesh::AddTriangle(size_t v1, size_t v2, size_t v3) {
  descriptors_.push_back(TriangleDescriptor(v1, v2, v3));
}

void Mesh::CreateElements(std::vector<std::unique_ptr<Element>>* target) const {
  DVLOG(2) << "Creating elements with " << vertices_.size() << " vertices and "
           << descriptors_.size() << " triangles";
  for (auto it = descriptors_.begin(); it != descriptors_.end(); ++it) {
    const Vertex* v1 = vertices_[it->v1_].get();
    const Vertex* v2 = vertices_[it->v2_].get();
    const Vertex* v3 = vertices_[it->v3_].get();

    target->push_back(std::unique_ptr<Element>(
        new Triangle(v1, v2, v3, material_)));
  }
}
