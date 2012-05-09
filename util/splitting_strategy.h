/*
 * Contains multiple splitting strategies which can be used while building a
 * KdTree.
 * Author: Dino Wernli
 */

#ifndef SPLITTING_STRATEGY_H_
#define SPLITTING_STRATEGY_H_

#include <vector>

#include "scene/element.h"
#include "util/axis.h"
#include "util/bounding_box.h"
#include "util/no_copy_assign.h"

struct SplitInformation {
  SplitInformation(bool should_split_, Axis split_axis_, Scalar split_position_)
      : should_split(should_split_), split_axis(split_axis_),
        split_position(split_position_) {}
  bool should_split;
  Axis split_axis;
  Scalar split_position;
};

class SplittingStrategy {
 protected:
  typedef std::vector<const Element*>& Elements;

 public:
  virtual ~SplittingStrategy() {}

  // Computes whether or not to split, a split axis and a split position based
  // on the passed information. If the conclusion is to not split, then the
  // rest of the data in the returned object is meaningless.
  virtual SplitInformation ComputeSplit(size_t depth, const BoundingBox& box,
                                        const Elements& elements) const = 0;
};

// This strategy just splits in the middle of the bounding box and cycles
// through the dimensions as the depth increases.
class MidpointSplit : public SplittingStrategy {
 public:
  MidpointSplit() {}
  virtual ~MidpointSplit() {}
  NO_COPY_ASSIGN(MidpointSplit);

  virtual SplitInformation ComputeSplit(size_t depth, const BoundingBox& box,
                                        const Elements& elements) const {
    Axis axis(depth);
    Scalar min = box.min()[axis];
    Scalar max = box.max()[axis];
    return SplitInformation(
        depth <= kTreeDepth && elements.size() >= kLeafSizeThreshold,
        axis, (min + max) / 2.0);
  }

 private:
  // If the root is at depth 0, no leaf will be at depth greater than this.
  const static size_t kTreeDepth = 20;

  // If the number of elements in a leaf goes below this threshold, it will not
  // be split.
  const static size_t kLeafSizeThreshold = 40;
};

#endif  /* SPLITTING_STRATEGY_H_ */
