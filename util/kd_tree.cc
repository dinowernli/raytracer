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
  void Split(Axis axis, size_t depth, const BoundingBox* box);

  // It is theoretically possible for leaves to have empty element vectors.
  bool IsLeaf() const { return left.get() == NULL && right.get() == NULL; }

  size_t NumElements() const {
    return IsLeaf() ? elements->size() :
        left->NumElements() + right->NumElements();
  }

  // Returns whether or not the ray intersects any of the elements. If data is
  // not NULL, data about the first intersection is stored.
  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  std::unique_ptr<std::vector<const Element*>> elements;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
};

KdTree::Node::Node() {
  this->elements.reset(new std::vector<const Element*>());
}

void KdTree::Node::Split(Axis axis, size_t depth, const BoundingBox* box) {
  CHECK(IsLeaf()) << "Split() can only be called on leaf nodes";
  if (box == NULL) {
    CHECK(elements->size() == 0) << "Invalid bounding box during split";
  }

  if (depth > kTreeDepth || elements->size() < kLeafSizeThreshold) {
    return;
  }

  // Create children.
  left.reset(new Node());
  right.reset(new Node());

  // TODO(dinow): Implement other strategies than MidPointSplit.
  Scalar split_pos = (box->min()[axis] + box->max()[axis]) / 2.0;

  // Move elements to either 1 or 2 relevant children.
  for (size_t i = 0; i < elements->size(); ++i) {
    if (elements->at(i)->bounding_box()->min()[axis] <= split_pos) {
      left->elements->push_back(elements->at(i));
    }
    if (elements->at(i)->bounding_box()->max()[axis] >= split_pos) {
      right->elements->push_back(elements->at(i));
    }
  }

  // Recursively split children.
  Point3 left_max = box->max();
  left_max[axis] = split_pos;
  BoundingBox left_box(box->min(), left_max);
  left->Split(axis.Next(), depth + 1, &left_box);

  Point3 right_min = box->max();
  right_min[axis] = split_pos;
  BoundingBox right_box(right_min, box->max());
  right->Split(axis.Next(), depth + 1, &right_box);

  // Clean up elements.
  elements.reset();
  CHECK(!IsLeaf()) << "KdTree node still leaf after split";
}

bool KdTree::Node::Intersect(const Ray& ray, IntersectionData* data) const {
  bool result = false;
  if (IsLeaf()) {
    for (size_t i = 0; i < elements->size(); ++i) {
      result = result | elements->at(i)->Intersect(ray, data);
      if (result && data == NULL) {
        return true;
      }
    }
  } else {
    bool left_intersection = left->Intersect(ray, data);
    if (left_intersection && data == NULL) {
      return true;
    }
    bool right_intersection = right->Intersect(ray, data);
    result = left_intersection || right_intersection;
  }
  return result;
}

KdTree::KdTree() {
}

KdTree::~KdTree() {
}

size_t KdTree::NumElementsWithDuplicates() const {
  if (root_.get() == NULL) {
    return 0;
  } else {
    return root_->NumElements();
  }
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

  LOG(INFO) << "Building KdTree for " << root_->elements->size() << " elements";

  root_->Split(kInitialSplitAxis, 0, bounding_box_.get());
}

bool KdTree::Intersect(const Ray& ray, IntersectionData* data) const {
  if (root_.get() == NULL) {
    LOG(WARNING) << "Called intersect on uninitialized KdTree. Returning false";
  }
  return root_->Intersect(ray, data);
}

// static
const Axis KdTree::kInitialSplitAxis = Axis::x();

// static
const size_t KdTree::kTreeDepth = 20;

// static
const size_t KdTree::kLeafSizeThreshold = 40;
