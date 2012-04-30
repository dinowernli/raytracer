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
           const Color3& specular, Scalar shininess)
      : emission_(emission), ambient_(ambient), diffuse_(diffuse),
        specular_(specular), shininess_(shininess) {
  }
  virtual ~Material() {
  }
  NO_COPY_ASSIGN(Material);

  const Color3& ambient() const { return ambient_; }
  const Color3& diffuse() const { return diffuse_; }
  const Color3& specular() const { return specular_; }
  const Color3& emission() const { return emission_; }
  const Scalar shininess() const { return shininess_; }

 private:
  Color3 emission_;
  Color3 ambient_;
  Color3 diffuse_;
  Color3 specular_;

  Scalar shininess_;
};

#endif  /* MATERIAL_H_ */
