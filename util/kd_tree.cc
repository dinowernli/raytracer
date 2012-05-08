/*
 * Author: Dino Wernli
 */

#include "kd_tree.h"

#include <glog/logging.h>

#include "renderer/intersection_data.h"
#include "scene/element.h"
#include "util/ray.h"

// Convenience method which takes care of linearly testing all elements for
// intersection.
static bool LinearIntersect(const std::vector<const Element*>& elements,
                            const Ray& ray, IntersectionData* data) {
  bool intersected = false;
  for (size_t i = 0; i < elements.size(); ++i) {
    intersected = elements[i]->Intersect(ray, data) | intersected;
    if (intersected && data == NULL) {
      return true;
    }
  }
  return intersected;
}


struct KdTree::Node {
  // Creates an empty leaf. Sets axis to X.
  Node();

  // Only to be called on leaves. Expects depth to be the current depth of the
  // leaf before splitting.
  void Split(Axis axis, size_t depth, const BoundingBox& box);

  // It is theoretically possible for leaves to have empty element vectors.
  bool IsLeaf() const { return left.get() == NULL && right.get() == NULL; }

  size_t NumElements() const {
    return IsLeaf() ? elements->size() :
        left->NumElements() + right->NumElements();
  }

  // Returns whether or not the ray intersects any of the elements. If data is
  // not NULL, data about the first intersection is stored.
  bool Intersect(const Ray& ray, Scalar t_near, Scalar t_far,
                 IntersectionData* data = NULL) const;

  std::unique_ptr<std::vector<const Element*>> elements;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  Scalar split_position;
  Axis split_axis;
};

KdTree::Node::Node() : split_axis(Axis::x()) {
  this->elements.reset(new std::vector<const Element*>());
}

void KdTree::Node::Split(Axis axis, size_t depth, const BoundingBox& box) {
  CHECK(IsLeaf()) << "Split() can only be called on leaf nodes";
  if (depth > kTreeDepth || elements->size() < kLeafSizeThreshold) {
    return;
  }

  // TODO(dinow): Implement other strategies than MidPointSplit.
  split_position = (box.min()[axis] + box.max()[axis]) / 2.0;
  split_axis = axis;
  left.reset(new Node());
  right.reset(new Node());

  // Move elements to either 1 or 2 relevant children.
  for (size_t i = 0; i < elements->size(); ++i) {
    if (elements->at(i)->bounding_box()->min()[axis] <= split_position) {
      left->elements->push_back(elements->at(i));
    }
    if (elements->at(i)->bounding_box()->max()[axis] >= split_position) {
      right->elements->push_back(elements->at(i));
    }
  }

  // Recursively split children.
  Point3 left_max = box.max();
  left_max[axis] = split_position;
  BoundingBox left_box(box.min(), left_max);
  left->Split(axis.Next(), depth + 1, left_box);

  Point3 right_min = box.min();
  right_min[axis] = split_position;
  BoundingBox right_box(right_min, box.max());
  right->Split(axis.Next(), depth + 1, right_box);

  // Clean up elements.
  elements.reset();
  CHECK(!IsLeaf()) << "KdTree node still leaf after split";
}

bool KdTree::Node::Intersect(const Ray& ray, Scalar t_near,
    Scalar t_far, IntersectionData* data) const {
  if (IsLeaf()) {
    return LinearIntersect(*elements, ray, data);
  }

  Scalar ray_direction_axis = ray.direction()[split_axis];
  Scalar ray_origin_axis = ray.origin()[split_axis];

  // Ray not moving in direction of the split, so only intersections with one
  // side are possible.
  if (ray_direction_axis == 0) {
    if (ray_origin_axis <= split_position) {
      return left->Intersect(ray, t_near, t_far, data);
    } else {
      return right->Intersect(ray, t_near, t_far, data);
    }
  }

  // Determine where on the ray the split happens.
  Scalar t_split = (split_position - ray_origin_axis) / ray_direction_axis;
  bool intersected = false;

  // Determine which is the first child traversed by the ray.
  const Node* first = left.get();
  const Node* second = right.get();
  if (ray_direction_axis < 0) std::swap(first, second);

  // Call recursively.
  if (t_split > t_far) {
    return first->Intersect(ray, t_near, t_far, data);
  } else if (t_split < t_near) {
    return second->Intersect(ray, t_near, t_far, data);
  } else if ((intersected = first->Intersect(ray, t_near, t_split, data))
             && (data == NULL || data->t < t_split)) {
      return true;
  } else {
    return second->Intersect(ray, t_split, t_far, data) || intersected;
  }
}

KdTree::KdTree() {
}

KdTree::~KdTree() {
}

size_t KdTree::NumElementsInLeaves() const {
  if (root_.get() == NULL) {
    return 0;
  } else {
    return root_->NumElements();
  }
}

void KdTree::Init(const std::vector<std::unique_ptr<Element>>& elements) {
  root_.reset(new Node());
  unbounded_elements_.clear();

  // TODO(dinow): Replace this once the hack below is removed.
  bounding_box_.reset(NULL);

  // Take the first bounded element as original box, and merge all the
  // following boxes.
  bool first = true;
  for (auto it = elements.begin(); it != elements.end(); ++it) {
    const Element* element = it->get();
    if (element->IsBounded()) {
      const BoundingBox& box = *element->bounding_box();
      // TODO(dinow): Remove this hack and replace with decent BB constructor.
      if (first) {
        bounding_box_.reset(new BoundingBox(box));
        first = false;
      } else {
        bounding_box_->Include(box);
      }
      root_->elements->push_back(element);
    } else {
      unbounded_elements_.push_back(element);
    }
  }

  const size_t n_bounded_elements = root_->elements->size();
  root_->Split(kInitialSplitAxis, 0, *bounding_box_.get());
  LOG(INFO) << "Built KdTree for " << n_bounded_elements
            << " bounded elements and " << unbounded_elements_.size()
            << " unbounded elements";
  LOG(INFO) << "Number of (bounded) elements in KdTree leaves is "
            << NumElementsInLeaves();
}

bool KdTree::Intersect(const Ray& ray, IntersectionData* data) const {
  if (root_.get() == NULL) {
    LOG(WARNING) << "Called intersect on uninitialized KdTree. Returning false";
    return false;
  }

  bool intersected = LinearIntersect(unbounded_elements_, ray, data);
  Scalar t_near, t_far;
  if (bounding_box_->Intersect(ray, &t_near, &t_far)) {
    intersected = root_->Intersect(ray, t_near, t_far, data) | intersected;
  }
  return intersected;
}

// static
const Axis KdTree::kInitialSplitAxis = Axis::x();

// static
const size_t KdTree::kTreeDepth = 20;

// static
const size_t KdTree::kLeafSizeThreshold = 40;
