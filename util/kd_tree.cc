/*
 * Author: Dino Wernli
 */

#include "kd_tree.h"

#include <glog/logging.h>

#include "renderer/intersection_data.h"
#include "scene/element.h"
#include "util/ray.h"

struct KdTree::Node {
  // Creates an empty leaf. Sets axis to X.
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
  bool Intersect(const Ray& ray, Scalar t_near, Scalar t_far,
                 IntersectionData* data = NULL) const;

  bool OwnRecursiveIntersect(const Ray& ray, Scalar t_near, Scalar t_far,
                             IntersectionData* data = NULL) const;

  bool NevRecursiveIntersect(const Ray& ray, Scalar t_near, Scalar t_far,
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

void KdTree::Node::Split(Axis axis, size_t depth, const BoundingBox* box) {
  CHECK(IsLeaf()) << "Split() can only be called on leaf nodes";
  if (box == NULL) {
    CHECK(elements->size() == 0) << "Invalid bounding box during split";
  }

  if (depth > kTreeDepth || elements->size() < kLeafSizeThreshold) {
    return;
  }

  // TODO(dinow): Implement other strategies than MidPointSplit.
  split_position = (box->min()[axis] + box->max()[axis]) / 2.0;
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
  Point3 left_max = box->max();
  left_max[axis] = split_position;
  BoundingBox left_box(box->min(), left_max);
  left->Split(axis.Next(), depth + 1, &left_box);

  Point3 right_min = box->max();
  right_min[axis] = split_position;
  BoundingBox right_box(right_min, box->max());
  right->Split(axis.Next(), depth + 1, &right_box);

  // Clean up elements.
  elements.reset();
  CHECK(!IsLeaf()) << "KdTree node still leaf after split";
}

bool KdTree::Node::OwnRecursiveIntersect(const Ray& ray, Scalar t_near,
    Scalar t_far, IntersectionData* data) const {
  if (IsLeaf()) {
    bool intersected = false;
    for (size_t i = 0; i < elements->size(); ++i) {
      intersected = intersected | elements->at(i)->Intersect(ray, data);
      if (intersected && data == NULL) {
        return true;
      }
    }
    return intersected;
  }

  Scalar ray_direction_axis = ray.direction()[split_axis];
  Scalar ray_origin_axis = ray.origin()[split_axis];
  bool is_origin_left = (ray_origin_axis <= split_position);

  // If ray is parallel to split axis, anyting the ray intersects with will
  // have to be on the side of the origin.
  if (ray_direction_axis == 0) {
    if (is_origin_left) {
      return left->Intersect(ray, t_near, t_far, data);
    } else {
      return right->Intersect(ray, t_near, t_far, data);
    }
  }

  // The near child is the child which is closer to the ray's origin in split
  // coordinates. The far child is the other one.
  const Node* near = is_origin_left ? left.get() : right.get();
  const Node* far = is_origin_left ? right.get() : left.get();

  Scalar t_split = (split_position - ray_origin_axis) / ray_origin_axis;
  if (t_split > t_far) {
    return near->Intersect(ray, t_near, t_far, data);
  } else if (t_split < t_near) {
    // TODO(dinow): Divergence between nev's implementation and mine. Need to
    // investigate further.
    /*
    if (ray.PointAt(t_near)[split_axis] < split_position) {
      return left->Intersect(ray, t_near, t_far, data);
    } else {
      return right->Intersect(ray, t_near, t_far, data);
    }
    */

    // TODO(dinow): It looks like the above is equivalent to the following
    // because this is simply the near/far test again using the point at t_near
    // instead of the origin.
    return far->Intersect(ray, t_near, t_far, data);
  } else {
    // This is the case t_near <= t_split <= t_far. Test both children.
    DVLOG(2) << "Intersecting both children";
    bool intersected = false;
    if ((intersected = near->Intersect(ray, t_near, t_split))
        && (data == NULL || data->t < t_split)) {
      return true;
    } else {
      return far->Intersect(ray, t_split, t_far, data) || intersected;
    }
  }
}

bool KdTree::Node::NevRecursiveIntersect(const Ray& ray, Scalar t_near,
    Scalar t_far, IntersectionData* data) const {
  if (IsLeaf()) {
    bool intersected = false;
    for (size_t i = 0; i < elements->size(); ++i) {
      intersected = intersected | elements->at(i)->Intersect(ray, data);
      if (intersected && data == NULL) {
        return true;
      }
    }
    return intersected;
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

  bool intersected = false;

  // Determine where on the ray the split happens.
  Scalar t_split = (split_position - ray_origin_axis) / ray_direction_axis;

  // Determine which is the first child traversed by the ray.
  const Node* first = left.get();
  const Node* second = right.get();
  if (ray_direction_axis < 0) std::swap(first, second);

  // Call recursively.
  if (t_split > t_far) {
    return first->Intersect(ray, t_near, t_far, data);
  } else if (t_split < t_near) {
    return second->Intersect(ray, t_near, t_far, data);
  } else if ((intersected |= first->Intersect(ray, t_near, t_split, data))) {
    if (data == NULL || data->t < t_split) {
      return true;
    }
  } else {
    intersected = second->Intersect(ray, t_split, t_far, data) || intersected;
  }
  return intersected;
}


bool KdTree::Node::Intersect(const Ray& ray, Scalar t_near, Scalar t_far,
                             IntersectionData* data) const {
  // TODO(dinow): Debug implementations and clean this up.
  //return OwnRecursiveIntersect(ray, t_near, t_far, data);
  return NevRecursiveIntersect(ray, t_near, t_far, data);
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

  if (bounding_box_.get() != NULL) {
    DVLOG(2) << "Built KdTree with bounding box " << *bounding_box_;
  }
}

bool KdTree::Intersect(const Ray& ray, IntersectionData* data) const {
  if (root_.get() == NULL) {
    LOG(WARNING) << "Called intersect on uninitialized KdTree. Returning false";
  }

  DVLOG(2) << "Intersecting with ray " << ray;

  Scalar t_near, t_far;
  if (!bounding_box_->Intersect(ray, &t_near, &t_far)) {
    DVLOG(2) << "Bounding box did not intersect";
    return false;
  }
  return root_->Intersect(ray, t_near, t_far, data);
}

// static
const Axis KdTree::kInitialSplitAxis = Axis::x();

// static
const size_t KdTree::kTreeDepth = 20;

// static
const size_t KdTree::kLeafSizeThreshold = 40;
