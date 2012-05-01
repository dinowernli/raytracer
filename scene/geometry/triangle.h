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
  virtual ~Triangle();
  NO_COPY_ASSIGN(Triangle);

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL);

 private:
  const Material* material_;
  const Vertex* vertex1_;
  const Vertex* vertex2_;
  const Vertex* vertex3_;
};

#endif  /* TRIANGLE_H_ */
