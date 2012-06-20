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
