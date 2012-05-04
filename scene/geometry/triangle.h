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
  // Allocates new vertices where the points and normals are copied. Does not
  // take ownership of any passed data. Any normal which is NULL is inferred as
  // well as possible.
  Triangle(const Point3& c1, const Point3& c2, const Point3& c3,
           const Material* material = NULL, const Vector3* n1 = NULL,
           const Vector3* n2 = NULL, const Vector3* n3 = NULL);

  // Takes no ownerhip of any passed pointers.
  Triangle(const Vertex* v1, const Vertex* v2, const Vertex* v3,
           const Material* material = NULL);

  virtual ~Triangle();
  NO_COPY_ASSIGN(Triangle);

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  const Vertex& vertex1() const { return *vertex1_; }
  const Vertex& vertex2() const { return *vertex2_; }
  const Vertex& vertex3() const { return *vertex3_; }

 private:
  const Material* material_;
  const Vertex* vertex1_;
  const Vertex* vertex2_;
  const Vertex* vertex3_;

  // TODO(dinow): Figure out if there is another way to decide whether or not
  // to delete the vertices in the destructor.
  bool owns_vertices_;
};

template<class OStream>
OStream& operator<<(OStream& os, const Triangle& t)
{
  return os << "(triangle: " << t.vertex1() << ", " << t.vertex2() << ", "
            << t.vertex3() << ")";
}

#endif  /* TRIANGLE_H_ */
