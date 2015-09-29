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
 * Represents the material out of which an element is made of.
 * Author: Dino Wernli
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "scene/texture/constant_texture.h"
#include "scene/texture/texture.h"
#include "util/color3.h"
#include "util/no_copy_assign.h"

class Material {
 public:
  Material(Texture* emission, Texture* ambient, Texture* diffuse,
           Texture* specular, Scalar shininess,
           Scalar reflection_percentage, Scalar refraction_percentage,
           Scalar refraction_index)
      : emission_(emission), ambient_(ambient), diffuse_(diffuse),
        specular_(specular), shininess_(shininess),
        reflection_percentage_(reflection_percentage),
        refraction_percentage_(refraction_percentage),
        refraction_index_(refraction_index) {
  }
  virtual ~Material() {
  }
  NO_COPY_ASSIGN(Material);

  // Returns a material suitable for visualization. The caller takes ownership
  // of the returned material.
  static Material* VisualizationMaterial(const Color3& color) {
    Color3 black(0, 0, 0);

    // TODO(dinow): Fix memory leak here.
    ConstantTexture* b = new ConstantTexture(black);
    ConstantTexture* c = new ConstantTexture(color);

    return new Material(c, b, b, b, 20, 0, 0.6, 1);
  }

  const Color3 emission(const IntersectionData& data) const {
    return emission_->Evaluate(data);
  }

  const Color3 ambient(const IntersectionData& data) const {
    return ambient_->Evaluate(data);
  }

  const Color3 diffuse(const IntersectionData& data) const {
    return diffuse_->Evaluate(data);
  }

  const Color3 specular(const IntersectionData& data) const {
    return specular_->Evaluate(data);
  }

  Scalar shininess() const { return shininess_; }
  Scalar reflection_percentage() const { return reflection_percentage_; }
  Scalar refraction_percentage() const { return refraction_percentage_; }
  Scalar refraction_index() const { return refraction_index_; }

 private:
  Texture* emission_;
  Texture* ambient_;
  Texture* diffuse_;
  Texture* specular_;

  Scalar shininess_;
  Scalar reflection_percentage_;
  Scalar refraction_percentage_;
  Scalar refraction_index_;
};

#endif  /* MATERIAL_H_ */
