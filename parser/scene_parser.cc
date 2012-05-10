/*
 * Author: Dino Wernli
 */

#include "scene_parser.h"

#include "proto/scene/scene_data.pb.h"
#include "scene/scene.h"

SceneParser::SceneParser() {
}

SceneParser::~SceneParser() {
}

// static
void SceneParser::ParseScene(const raytracer::SceneData& data, Scene* scene) {
  // TODO(dinow): Do some parsing, add stuff to the scene.
}
