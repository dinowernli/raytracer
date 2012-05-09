/*
 * A 3D hierarchical intersection data structure which accepts all elements,
 * including elements which are not spacially bounded. The latter are held in a
 * separate structure and are considered during intersection. Elements are only
 * contained in leaves of the tree.
 *
 * Author: Dino Wernli
 */

#ifndef KD_TREE_H_
#define KD_TREE_H_

#include <memory>
#include <vector>

#include "util/axis.h"
#include "util/bounding_box.h"
#include "util/splitting_strategy.h"

class Element;
class IntersectionData;
class Ray;

class KdTree {
 public:
  // Takes ownership of the passed SplittingStrategy.
  KdTree(SplittingStrategy* strategy = new MidpointSplit());
  virtual ~KdTree();

  // Builds a tree which contains pointers to the passed elements. No ownership
  // is taken for any of the elements, none of the elements will be changed.
  void Init(const std::vector<std::unique_ptr<Element>>& elements);

  // Returns whether or not the ray intersects any of the elements. If data is
  // not NULL, data about the first intersection is stored. If init has not
  // been called, this returns false.
  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

 private:
  struct Node;
  std::unique_ptr<Node> root_;

  // Returns the total number of elements stored in all leaves of the tree.
  // Note that this potentially elements multiple times if they were added to
  // the left and to the right.
  size_t NumElementsInLeaves() const;

  // A bounding box which contains all bounded elements of the tree.
  std::unique_ptr<BoundingBox> bounding_box_;

  // A separate container for all elements which are not bounded.
  std::vector<const Element*> unbounded_elements_;

  std::unique_ptr<SplittingStrategy> strategy_;
};

#endif  /* KD_TREE_H_ */
