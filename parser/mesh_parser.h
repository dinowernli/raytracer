/*
 * Author: Dino Wernli
 */

#ifndef MESH_PARSER_H_
#define MESH_PARSER_H_

#include<string>
#include<vector>

#include "util/point3.h"
#include "util/vector3.h"

class Mesh;

class MeshParser {
 public:
  MeshParser();
  virtual ~MeshParser();

  void ClearState();

  // Parses the file and returns a new mesh with the contents. The caller takes
  // ownership of the returned pointer.
  Mesh* LoadFile(const std::string& path);

  // TODO(dinow): Change the triangles to have corners and normals. That way,
  // the "obj" format can be parsed properly.
  std::vector<Point3> points_;
  std::vector<Vector3> normals_;

  static const std::string kComment;
  static const std::string kNormal;
  static const std::string kVertex;
  static const std::string kTriangle;
};

#endif  /* MESH_PARSER_H_ */
