/*
 * Represents the material out of which an element is made of.
 * Author: Dino Wernli
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "util/color3.h"
#include "util/no_copy_assign.h"

class Material {
 public:
  Material(const Color3& emission, const Color3& ambient, const Color3& diffuse,
           const Color3& specular, Scalar shininess,
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
    return new Material(color, black, black, black, 20, 0, 0.6, 1);
  }

  const Color3& emission() const { return emission_; }
  const Color3& ambient() const { return ambient_; }
  const Color3& diffuse() const { return diffuse_; }
  const Color3& specular() const { return specular_; }
  Scalar shininess() const { return shininess_; }
  Scalar reflection_percentage() const { return reflection_percentage_; }
  Scalar refraction_percentage() const { return refraction_percentage_; }
  Scalar refraction_index() const { return refraction_index_; }

 private:
  Color3 emission_;
  Color3 ambient_;
  Color3 diffuse_;
  Color3 specular_;

  Scalar shininess_;

  Scalar reflection_percentage_;
  Scalar refraction_percentage_;
  Scalar refraction_index_;
};

#endif  /* MATERIAL_H_ */
