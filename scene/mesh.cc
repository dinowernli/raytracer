/*
 * Author: Dino Wernli
 */

#include "mesh.h"

#include <glog/logging.h>

#include "scene/element.h"
#include "scene/geometry/triangle.h"
#include "util/bounding_box.h"
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

    Triangle* triangle = new Triangle(v1, v2, v3, material_);
    DVLOG(3) << "Adding triangle " << *triangle;
    target->push_back(std::unique_ptr<Element>(triangle));
  }
}

void Mesh::Transform(Scalar scale, const Vector3& translation) {
  if (vertices_.size() == 0) {
    return;
  }

  BoundingBox box;
  for (size_t i = 0; i < vertices_.size(); ++i) {
    box.Include(vertices_[i]->point());
  }

  Point3 center = box.min() + 0.5 * (box.min().VectorTo(box.max()));
  Scalar radius = Point3::SquaredDistance(center, box.max());

  for (size_t i = 0; i < vertices_.size(); ++i) {
    Vertex& vertex = *(vertices_[i]);
    const Vector3 center_vector = center.VectorFromOrigin();
    const Scalar factor = scale / radius;
    vertex.set_point(factor * (vertex.point() - center_vector) + translation);
  }
}

void Mesh::InferNormals() {
  for(size_t i = 0; i < vertices_.size(); ++i) {
    vertices_[i]->set_normal(Vector3(0, 0, 0));
  }

  for(size_t i = 0; i < descriptors_.size(); ++i) {
    Vertex& v1 = *vertices_[descriptors_[i].v1_];
    Vertex& v2 = *vertices_[descriptors_[i].v2_];
    Vertex& v3 = *vertices_[descriptors_[i].v3_];

    Vector3 normal = v1.point().VectorTo(v2.point())
        .Cross(v1.point().VectorTo(v3.point())).Normalize();

    v1.set_normal(v1.normal() + normal);
    v2.set_normal(v2.normal() + normal);
    v3.set_normal(v3.normal() + normal);
  }

  for(size_t i = 0; i < vertices_.size(); ++i) {
    Vertex& vertex = *vertices_[i];
    vertex.set_normal(vertex.normal().Normalized());
  }
}
