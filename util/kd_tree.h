/*
 * A 3D hierarchical intersection data structure. Elements are only contained
 * in leaves of the tree.
 *
 * TODO(dinow): Extract some sort of SplittingStrategy notion.
 *
 * Author: Dino Wernli
 */

#ifndef KD_TREE_H_
#define KD_TREE_H_

#include <memory>
#include <vector>

#include "util/bounding_box.h"

class Element;
class IntersectionData;
class Ray;

class KdTree {
 public:
  KdTree();
  virtual ~KdTree();

  // Builds a tree which contains pointers to the passed elements. No ownership
  // is taken for any of the elements, none of the elements will be changed.
  // Only elements which are bounded will be incorporated in the tree.
  void Init(const std::vector<std::unique_ptr<Element>>& elements);

  // Returns whether or not the ray intersects any of the elements. If data is
  // not NULL, data about the first intersection is stored. If init has not
  // been called, this returns false.
  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  // Returns the total number of elements stored in all leaves of the tree.
  // Note that this potentially elements multiple times if they were added to
  // the left and to the right.
  size_t NumElementsWithDuplicates() const;

 private:
  struct Node;
  std::unique_ptr<Node> root_;
  std::unique_ptr<BoundingBox> bounding_box_;

  // TODO(dinow): Create an enum for axis somewhere.
  const static size_t kInitialSplitAxis;

  // If the root is at depth 0, no leaf will be at depth greater than this.
  const static size_t kTreeDepth;

  // If the number of elements in a leaf goes below this threshold, it will not
  // be split.
  const static size_t kLeafSizeThreshold;
};

#endif  /* KD_TREE_H_ */
