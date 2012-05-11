/*
 * Takes care of parsing the items of the scene.
 * Author: Dino Wernli
 */

#ifndef SCENE_PARSER_H_
#define SCENE_PARSER_H_

#include "util/no_copy_assign.h"

namespace raytracer {
class SceneData;
}

class Scene;

class SceneParser {
 public:
  SceneParser();
  virtual ~SceneParser();
  NO_COPY_ASSIGN(SceneParser);

  // Parses data and add everything to scene.
  static void ParseScene(const raytracer::SceneData& data, Scene* scene);
};

#endif  /* SCENE_PARSER_H_ */
