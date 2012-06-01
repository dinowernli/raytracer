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
}

class Material;
class Scene;

class SceneParser {
 public:
  SceneParser();
  virtual ~SceneParser();
  NO_COPY_ASSIGN(SceneParser);

  // Methods used for parsing entities from protos. The caller takes ownership
  // of the returned pointer. If parsing failed, returns NULL for pointers and
  // the default value for value types.
  static Material* Parse(const raytracer::MaterialData& data);
  static Color3 Parse(const raytracer::ColorData& data);
  static Vector3 Parse(const raytracer::VectorData& data);
  static Point3 Parse(const raytracer::PointData& data);

  // Parses data and add everything to scene.
  void ParseScene(const raytracer::SceneData& data, Scene* scene);

 private:
  // Fetches the material from the material map, returns NULL if it is missing.
  Material* GetMaterial(const std::string& id) const;

  // Maps material identifiers to materials.
  std::map<std::string, Material*> material_map_;
};

#endif  /* SCENE_PARSER_H_ */
