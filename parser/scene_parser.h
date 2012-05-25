/*
 * Takes care of parsing the items of the scene.
 * Author: Dino Wernli
 */

#ifndef SCENE_PARSER_H_
#define SCENE_PARSER_H_

#include <map>
#include <memory>
#include <string>

#include "util/no_copy_assign.h"

namespace raytracer {
class MaterialData;
class SceneData;
}

class Material;
class Scene;

class SceneParser {
 public:
  SceneParser();
  virtual ~SceneParser();
  NO_COPY_ASSIGN(SceneParser);

  // Parses a material from the proto. The caller takes ownership of the
  // returned material. Returns NULL if the data could not be parsed.
  Material* ParseMaterial(const raytracer::MaterialData& data);

  // Parses data and add everything to scene.
  void ParseScene(const raytracer::SceneData& data, Scene* scene);

 private:
  // Fetches the material from the material map, returns NULL if it is missing.
  Material* GetMaterial(const std::string& id) const;

  // Maps material identifiers to materials.
  std::map<std::string, Material*> material_map_;
};

#endif  /* SCENE_PARSER_H_ */
