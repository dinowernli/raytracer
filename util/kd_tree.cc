/*
 * Author: Dino Wernli
 */

#include "kd_tree.h"

#include <glog/logging.h>

#include "renderer/intersection_data.h"
#include "scene/element.h"
#include "util/ray.h"

struct KdTree::Node {
  // Creates an empty leaf.
  Node();

  // Only to be called on leaves. Expects depth to be the current depth of the
  // leaf before splitting.
  void Split(size_t axis, size_t depth, const BoundingBox* box);

  // It is theoretically possible for leaves to have empty element vectors.
  bool IsLeaf() const { return left.get() == NULL && right.get() == NULL; }

  std::unique_ptr<std::vector<const Element*>> elements;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
};

KdTree::Node::Node() {
  this->elements.reset(new std::vector<const Element*>());
}

void KdTree::Node::Split(size_t axis, size_t depth, const BoundingBox* box) {
  CHECK(IsLeaf()) << "Split() can only be called on leaf nodes";
  if (box == NULL) {
    CHECK(elements->size() == 0) << "Invalid bounding box during split";
  }

  if (depth > kTreeDepth || elements->size() < kLeafSizeThreshold) {
    return;
  }

  // TODO(dinow): Distribute elements to children.
}

KdTree::KdTree() {
}

KdTree::~KdTree() {
}

void KdTree::Init(const std::vector<std::unique_ptr<Element>>& elements) {
  root_.reset(new Node());

  // Take the first bounded element as original box, and merge all the
  // following boxes.
  bool first = true;
  for (auto it = elements.begin(); it != elements.end(); ++it) {
    const Element* element = it->get();
    if (element->IsBounded()) {
      const BoundingBox& box = *element->bounding_box();
      if (first) {
        bounding_box_.reset(new BoundingBox(box));
        first = false;
      } else {
        bounding_box_->Include(box);
      }
      root_->elements->push_back(element);
    }
  }

  root_->Split(kInitialSplitAxis, 0, bounding_box_.get());
}

bool KdTree::Intersect(const Ray& ray, IntersectionData* data) const {
  return false;
}

// static
const size_t KdTree::kInitialSplitAxis = 0;

// static
const size_t KdTree::kTreeDepth = 20;

// static
const size_t KdTree::kLeafSizeThreshold = 40;
