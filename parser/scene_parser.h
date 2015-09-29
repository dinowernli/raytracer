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
 * Takes care of parsing the items of the scene.
 * Author: Dino Wernli
 */

#ifndef SCENE_PARSER_H_
#define SCENE_PARSER_H_

#include <map>
#include <memory>
#include <string>

#include "util/color3.h"
#include "util/no_copy_assign.h"
#include "util/point3.h"
#include "util/vector3.h"

namespace raytracer {
class MaterialData;
class SceneData;
class TextureData;
}

class Material;
class Scene;
class Texture;

class SceneParser {
 public:
  SceneParser();
  virtual ~SceneParser();
  NO_COPY_ASSIGN(SceneParser);

  // Methods used for parsing entities from protos. The caller takes ownership
  // of the returned pointer. If parsing failed, returns NULL for pointers and
  // the default value for value types. If any textures need to be created,
  // they are added to scene.
  static Texture* Parse(const raytracer::TextureData& data);
  static Color3 Parse(const raytracer::ColorData& data);
  static Vector3 Parse(const raytracer::VectorData& data);
  static Point3 Parse(const raytracer::PointData& data);

  Material* Parse(const raytracer::MaterialData& data);

  // Parses data and add everything to scene.
  void ParseScene(const raytracer::SceneData& data, Scene* scene);

 private:
  // Fetches the material from the material map, returns NULL if it is missing.
  Material* GetMaterial(const std::string& id) const;

  // Maps material identifiers to materials.
  std::map<std::string, Material*> material_map_;
  std::map<std::string, Texture*> texture_map_;
};

#endif  /* SCENE_PARSER_H_ */
