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
 * Author: Dino Wernli
 */

#include "phong_shader.h"

#include <cmath>
#include <glog/logging.h>
#include <memory>

#include "renderer/intersection_data.h"
#include "scene/light/point_light.h"
#include "scene/material.h"
#include "scene/scene.h"

PhongShader::PhongShader(bool shadows) : shadows_(shadows) {
}

PhongShader::~PhongShader() {
}

Color3 PhongShader::Shade(const IntersectionData& data, const Scene& scene) {
  const Material& material = *data.material;
  Color3 emission(material.emission(data).Clamped());
  Color3 ambient((material.ambient(data) * scene.ambient()).Clamped());
  Color3 diffuse(0, 0, 0);
  Color3 specular(0, 0, 0);

  const std::vector<std::unique_ptr<Light>>& lights = scene.lights();
  for (auto it = lights.begin(); it != lights.end(); ++it) {
    const Light* light = it->get();
    Ray light_ray = light->GenerateRay(data.position);

    // Ignore the contribution from this light if it is occluded. Note that even
    // occlusion by another light counts as occlusion because all lights are
    // considered, therefore the other light will get its turn as well.
    if (shadows_ && scene.Intersect(light_ray)) {
      continue;
    }

    Vector3 point_to_light = -1 * light_ray.direction();
    const Point3& cam_pos = scene.camera().position();
    Vector3 point_to_camera = data.position.VectorTo(cam_pos).Normalized();
    Vector3 normal = data.normal.Normalized();
    Scalar prod = point_to_light.Dot(normal);

    // Flip normal if the light is inside the element.
    if (prod < 0) {
      normal = -normal;
    }

    // Clamping seems to be necessary, otherwise some images get dark.
    Color3 diff = material.diffuse(data) * light->color();
    diffuse += (diff * prod).Clamped();

    // Add specular contribution.
    Color3 spec = material.specular(data) * light->color();
    Vector3 reflection = (-point_to_light).ReflectedOnPlane(normal);

    // Flip reflection if the camera is not on the same side of the element as
    // the light.
    if (prod < 0) {
      reflection = -reflection;
    }

    // Prevent cosine from turning positive through exponentiation.
    Scalar cosine = reflection.Dot(point_to_camera);
    cosine = cosine < 0 ? 0 : cosine;
    cosine = cosine > 1 ? 1 : cosine;

    // Again, clamping seems to be necessary, otherwise some images get dark.
    specular += (spec * pow(cosine, material.shininess())).Clamped();
  }
  return (emission + ambient + diffuse + specular).Clamped();
}
