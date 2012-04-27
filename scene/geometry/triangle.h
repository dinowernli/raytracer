/*
 * Author: Dino Wernli
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "scene/element.h"
#include "util/point3.h"

class Triangle : public Element {
 public:
  Triangle(const Point3& c1, const Point3& c2, const Point3& c3)
      : corners_({c1, c2, c3}) {
  }
  virtual ~Triangle() { }

  virtual bool Intersect(const Ray& ray, IntersectionData* data = NULL);

 private:
  Point3 corners_[3];
};



#endif  /* TRIANGLE_H_ */
