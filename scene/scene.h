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
 * Contains elements, lights etc. and can be rendered.
 * Author: Dino Wernli
 */

#ifndef SCENE_H_
#define SCENE_H_

#include<memory>
#include<vector>

#include "scene/camera.h"
#include "util/color3.h"
#include "util/kd_tree.h"
#include "util/no_copy_assign.h"

class Element;
class IntersectionData;
class Light;
class Material;
class Mesh;
class Ray;
class Texture;

namespace raytracer {
class SceneConfig;
}

class Scene {
 public:
  // If passed a KdTree, will use it to test intersection with all bounded
  // objects. Takes ownership of the passed KdTree.
  explicit Scene(KdTree* kd_tree = NULL);
  virtual ~Scene();
  NO_COPY_ASSIGN(Scene);

  // Takes ownership of the passed element.
  void AddElement(Element* element);

  // Takes ownership of the pass light.
  void AddLight(Light* light);

  // Takes ownership of the passed material.
  void AddMaterial(Material* material);

  // Takes ownership of the passed texture.
  void AddTexture(Texture* texture);

  // Takes ownership of the passed mesh. Extracts all elements of the mesh and
  // adds them to the list of elements.
  void AddMesh(Mesh* mesh);

  // Takes ownership of the passed camera.
  void set_camera(Camera* camera) { camera_.reset(camera); }
  const Camera& camera() const { return *camera_; }
  void set_background(const Color3& background) { background_ = background; }
  const Color3& background() const { return background_; }
  void set_ambient(const Color3& ambient) { ambient_ = ambient; }
  const Color3& ambient() const { return ambient_; }
  void set_refraction_index(Scalar index) { refraction_index_ = index; }
  Scalar refraction_index() const { return refraction_index_; }

  bool UsesKdTree() const { return kd_tree_.get() != NULL; }

  // Prepares the scene, builds data structures etc. Must be called before
  // before querying for intersections. If anything is added to the scene after
  // a call to Init(), it might be ignored until the next Init() call.
  void Init();

  bool Intersect(const Ray& ray, IntersectionData* data = NULL) const;

  // TODO(dinow): Figure out how to return something which only allows iteration
  // over const Light& (without an extra memory allocation).
  const std::vector<std::unique_ptr<Light>>& lights() const { return lights_; }

  static Scene* FromConfig(const raytracer::SceneConfig& config);

 private:
  std::vector<std::unique_ptr<Element>> elements_;
  std::vector<std::unique_ptr<Light>> lights_;
  std::unique_ptr<Camera> camera_;
  std::vector<std::unique_ptr<Material>> materials_;
  std::vector<std::unique_ptr<Mesh>> meshes_;
  std::vector<std::unique_ptr<Texture>> textures_;
  std::unique_ptr<KdTree> kd_tree_;

  Color3 background_;
  Color3 ambient_;
  Scalar refraction_index_;
};

#endif  /* SCENE_H_ */
