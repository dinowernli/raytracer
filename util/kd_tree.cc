/*
 * Author: Dino Wernli
 */

#include "kd_tree.h"

#include <glog/logging.h>

#include "parser/scene_parser.h"
#include "proto/config/scene_config.pb.h"
#include "renderer/intersection_data.h"
#include "scene/element.h"
#include "scene/geometry/triangle.h"
#include "scene/material.h"
#include "util/ray.h"

// Convenience method which takes care of linearly testing all elements for
// intersection.
static bool LinearIntersect(const std::vector<const Element*>& elements,
                            const Ray& ray, IntersectionData* data) {
  bool intersected = false;
  for (size_t i = 0; i < elements.size(); ++i) {
    intersected = elements[i]->Intersect(ray, data) || intersected;
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
  void Split(size_t depth, const BoundingBox& box,
             const SplittingStrategy& strategy, int visualization_depth,
             const Material* visualization_material,
             std::vector<Triangle*>* visualization_elements);

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

void KdTree::Node::Split(size_t depth, const BoundingBox& box,
                         const SplittingStrategy& strategy, int v_depth,
                         const Material* v_material,
                         std::vector<Triangle*>* v_elements) {
  CHECK(IsLeaf()) << "Split() can only be called on leaf nodes";
  SplitInformation info = strategy.ComputeSplit(depth, box, *elements);

  if (!info.should_split) {
    return;
  }

  split_position = info.split_position;
  split_axis = info.split_axis;
  left.reset(new Node());
  right.reset(new Node());

  // Move elements to either 1 or 2 relevant children.
  for (size_t i = 0; i < elements->size(); ++i) {
    if (elements->at(i)->bounding_box()->min()[split_axis] <= split_position) {
      left->elements->push_back(elements->at(i));
    }
    if (elements->at(i)->bounding_box()->max()[split_axis] >= split_position) {
      right->elements->push_back(elements->at(i));
    }
  }

  // Recursively split children.
  Point3 left_max = box.max();
  left_max[split_axis] = split_position;
  BoundingBox left_box(box.min(), left_max);

  Point3 right_min = box.min();
  right_min[split_axis] = split_position;
  BoundingBox right_box(right_min, box.max());

  // Add some visualization planes to children.
  if ((int)depth <= v_depth) {
    Point3 p1(left_max);
    Point3 p2(right_min);

    Axis next = split_axis.Next();
    Point3 p3(left_max);
    p3[next] = right_min[next];

    Axis prev = split_axis.Next().Next();
    Point3 p4(left_max);
    p4[prev] = right_min[prev];

    Triangle* t1 = new Triangle(p1, p2, p3, *v_material);
    Triangle* t2 = new Triangle(p1, p2, p4, *v_material);
    v_elements->push_back(t1);
    right->elements->push_back(t1);
    left->elements->push_back(t1);

    v_elements->push_back(t2);
    right->elements->push_back(t2);
    left->elements->push_back(t2);
  }

  // Split children an clean up.
  left->Split(depth + 1, left_box, strategy, v_depth, v_material, v_elements);
  right->Split(depth + 1, right_box, strategy, v_depth, v_material, v_elements);
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

KdTree::KdTree(SplittingStrategy* strategy, int visualization_depth,
                 Material* vistualization_material)
    : strategy_(strategy), visualization_depth_(visualization_depth),
      visualization_material_(vistualization_material) {
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

void KdTree::Init(std::vector<std::unique_ptr<Element>>* elements) {
  root_.reset(new Node());
  bounding_box_.reset(new BoundingBox());
  unbounded_elements_.clear();

  for (auto it = elements->begin(); it != elements->end(); ++it) {
    const Element* element = it->get();
    if (element->IsBounded()) {
      bounding_box_->Include(*element->bounding_box());
      root_->elements->push_back(element);
    } else {
      unbounded_elements_.push_back(element);
    }
  }

  std::vector<Triangle*> visualization_elements;
  const size_t n_bounded_elements = root_->elements->size();
  root_->Split(0, *bounding_box_, *strategy_, visualization_depth_,
               visualization_material_.get(), &visualization_elements);
  LOG(INFO) << "Built KdTree for " << n_bounded_elements
            << " bounded elements and " << unbounded_elements_.size()
            << " unbounded elements";
  LOG(INFO) << "Number of (bounded) elements in KdTree leaves is "
            << NumElementsInLeaves();

  // Add visualization elements to scene in order for them to get cleaned up
  // eventually.
  for (size_t i = 0; i < visualization_elements.size(); ++i) {
    std::unique_ptr<Element> element(visualization_elements[i]);
    elements->push_back(std::move(element));
  }
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
KdTree* KdTree::FromConfig(const raytracer::KdTreeConfig& config) {
  KdTree* tree = NULL;
  if (config.splitting_strategy() == raytracer::KdTreeConfig::MIDPOINT) {
    int v_depth = config.visualization_depth();
    if (v_depth < 0) {
      // Passing NULL as material is ok since it will never be used.
      tree = new KdTree(new MidpointSplit(), v_depth, NULL);
    } else {
      // Attempt to fetch the material.
      if (!config.has_visualization_color()) {
        LOG(WARNING) << "Could not load KdTree visualization material, "
                     << "deactivating visualization";
        tree = new KdTree(new MidpointSplit(), -1, NULL);
      } else {
        Color3 color = SceneParser::Parse(config.visualization_color());
        Material* v_material = Material::VisualizationMaterial(color);
        tree = new KdTree(new MidpointSplit(), v_depth, v_material);
      }
    }
  } else {
    LOG(WARNING) << "Unknown KdTree splitting strategy, skipping KdTree";
  }
  return tree;
}

