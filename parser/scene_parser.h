/*
 * Takes care of parsing the items of the scene.
 * Author: Dino Wernli
 */

#ifndef SCENE_PARSER_H_
#define SCENE_PARSER_H_

namespace raytracer {
class SceneData;
}

class Scene;

class SceneParser {
 public:
  SceneParser();
  virtual ~SceneParser();

  // Parses data and add everything to scene.
  static void ParseScene(const raytracer::SceneData& data, Scene* scene);
};

#endif  /* SCENE_PARSER_H_ */
