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

size_t Mesh::AddPoint(const Point3& point) {
  points_.push_back(point);
  return points_.size() - 1;
}

size_t Mesh::AddNormal(const Vector3& normal) {
  normals_.push_back(normal);
  return normals_.size() - 1;
}

void Mesh::AddTriangle(size_t v1, size_t n1, size_t v2, size_t n2, size_t v3,
                       size_t n3) {
  descriptors_.push_back(TriangleDescriptor(v1, n1, v2, n2, v3, n3));
}

void Mesh::CreateElements(std::vector<std::unique_ptr<Element>>* target) const {
  DVLOG(2) << "Creating " << descriptors_.size() << " triangles from mesh";
  for (auto it = descriptors_.begin(); it != descriptors_.end(); ++it) {
    const Point3* p1 = &points_[it->p1];
    const Point3* p2 = &points_[it->p2];
    const Point3* p3 = &points_[it->p3];

    const Vector3* n1 = &normals_[it->n1];
    const Vector3* n2 = &normals_[it->n2];
    const Vector3* n3 = &normals_[it->n3];

    Triangle* triangle = new Triangle(p1, p2, p3, n1, n2, n3, *material_);
    DVLOG(3) << "Adding triangle " << *triangle;
    target->push_back(std::unique_ptr<Element>(triangle));
  }
}

void Mesh::Transform(Scalar scale, const Vector3& translation) {
  BoundingBox box;
  for (size_t i = 0; i < points_.size(); ++i) {
    box.Include(points_[i]);
  }

  const Point3 center = box.min() + 0.5 * (box.min().VectorTo(box.max()));
  const Vector3 center_vector = center.VectorFromOrigin();
  const Scalar factor = scale / Point3::SquaredDistance(center, box.max());

  for (size_t i = 0; i < points_.size(); ++i) {
    points_[i].ReplaceWith(factor * (points_[i] - center_vector) + translation);
  }
}

void Mesh::InferNormals() {
  for(size_t i = 0; i < normals_.size(); ++i) {
    normals_[i].ReplaceWith(Vector3(0, 0, 0));
  }

  for(size_t i = 0; i < descriptors_.size(); ++i) {
    const Point3& p1 = points_[descriptors_[i].p1];
    const Point3& p2 = points_[descriptors_[i].p2];
    const Point3& p3 = points_[descriptors_[i].p3];

    Vector3 n = p1.VectorTo(p2).Cross(p1.VectorTo(p3)).Normalize();

    normals_[descriptors_[i].n1].ReplaceWith(normals_[descriptors_[i].n1] + n);
    normals_[descriptors_[i].n2].ReplaceWith(normals_[descriptors_[i].n2] + n);
    normals_[descriptors_[i].n3].ReplaceWith(normals_[descriptors_[i].n3] + n);
  }

  for(size_t i = 0; i < normals_.size(); ++i) {
    normals_[i].Normalize();
  }
}
