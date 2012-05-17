/*
 * A triangle element which can be added to a scene.
 * Author: Dino Wernli
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "scene/element.h"
#include "scene/geometry/vertex.h"
#include "util/no_copy_assign.h"

class Material;

class Triangle : public Element {
 public:
  // Copies the points and normals. Does not take ownership of any passed data.
  // Any normal which is NULL is inferred as well as possible.
  Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
           const Material& material, const Vector3* n1 = NULL,
           const Vector3* n2 = NULL, const Vector3* n3 = NULL);

  // Takes no ownership of the passed data, all pointers are simply copied.
  Triangle(const Point3* c1, const Point3* c2, const Point3* c3,
           const Vector3* n1, const Vector3* n2, const Vector3* n3,
           const Material& material);

  virtual ~Triangle();
  NO_COPY_ASSIGN(Triangle);

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  const Vertex& vertex1() const { return *vertex1_; }
  const Vertex& vertex2() const { return *vertex2_; }
  const Vertex& vertex3() const { return *vertex3_; }

 private:
  std::unique_ptr<Vertex> vertex1_;
  std::unique_ptr<Vertex> vertex2_;
  std::unique_ptr<Vertex> vertex3_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Triangle& t)
{
  return os << "(triangle: " << t.vertex1() << ", " << t.vertex2() << ", "
            << t.vertex3() << ")";
}

#endif  /* TRIANGLE_H_ */
