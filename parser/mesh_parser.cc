/*
 * Author: Dino Wernli
 */

#include "mesh_parser.h"

#include <fstream>
#include <glog/logging.h>
#include <sstream>

#include "scene/mesh.h"

// Takes a string of the form "NUMBER1//NUMBER2" and returns NUMBER.
void ExtractIndex(const std::string& index_str, size_t* n1, size_t* n2) {
  std::stringstream string_stream(index_str);
  string_stream >> *n1;
  string_stream.ignore(2);
  string_stream >> *n2;
}

MeshParser::MeshParser() {
}

MeshParser::~MeshParser() {
}

void MeshParser::ClearState() {
  points_.clear();
  normals_.clear();
}

Mesh* MeshParser::LoadFile(const std::string& path) {
  ClearState();
  Mesh* mesh = new Mesh();

  std::ifstream file_stream(path);
  while (!file_stream.eof()) {
    std::string type;
    file_stream >> type;

    if (type == kComment) {
      continue;
    } else if (type == kNormal) {
      Scalar x, y, z;
      file_stream >> x >> y >> z;
      normals_.push_back(Vector3(x, y, z));
    } else if (type == kVertex) {
      Scalar x, y, z;
      file_stream >> x >> y >> z;
      points_.push_back(Point3(x, y, z));
    } else if (type == kTriangle) {
      size_t v1, v2, v3, n1, n2, n3;
      std::string index1_str, index2_str, index3_str;
      file_stream >> index1_str >> index2_str >> index3_str;

      ExtractIndex(index1_str, &v1, &n1);
      ExtractIndex(index1_str, &v2, &n2);
      ExtractIndex(index1_str, &v3, &n3);

      CHECK(v1 == n1 && v2 == n2 && v3 == n3)
          << "Position and normal indices do not match.";
      mesh->AddTriangle(v1 - 1, v2 - 1, v3 - 1);
    }
  }

  return mesh;
}

// static
const std::string MeshParser::kComment = "#";

// static
const std::string MeshParser::kNormal = "vn";

// static
const std::string MeshParser::kVertex = "v";

// static
const std::string MeshParser::kTriangle = "f";
