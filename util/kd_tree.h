// The MIT License (MIT)
//
// Copyright (c) 2015 dinowernli
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

namespace raytracer {
class KdTreeConfig;
}

class KdTree {
 public:
  // Takes ownership of the passed SplittingStrategy and material. The KdTree
  // will add visualization planes for all levels <= visualization_depth. For no
  // visualization at all, pass -1 as depth.
  KdTree(SplittingStrategy* strategy, int visualization_depth,
          Material* visualization_material = NULL);
  virtual ~KdTree();

  // Builds a tree which contains pointers to the passed elements. No ownership
  // is taken for any of the elements, none of the elements will be changed.
  // Any visualization triangles created are added to elements.
  void Init(std::vector<std::unique_ptr<Element>>* elements);

  // Returns whether or not the ray intersects any of the elements. If data is
  // not NULL, data about the first intersection is stored. If init has not
  // been called, this returns false.
  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  static KdTree* FromConfig(const raytracer::KdTreeConfig& config);

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

  int visualization_depth_;
  std::unique_ptr<Material> visualization_material_;
};

#endif  /* KD_TREE_H_ */
