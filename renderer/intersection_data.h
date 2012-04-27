/*
 * Contains the information about an intersection.
 * Author: Dino Wernli
 */

#ifndef INTERSECTIONDATA_H_
#define INTERSECTIONDATA_H_

#include "scene/element.h"
#include "util/point3.h"

struct IntersectionData {
  IntersectionData() : t(0), element(NULL) {}

  Scalar t;
  Point3 position;
  const Element* element;
};

#endif /* INTERSECTIONDATA_H_ */
