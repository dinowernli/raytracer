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

#include "mesh_parser.h"

#include <fstream>
#include <memory>
#include <sstream>

#include "scene/mesh.h"

// Takes a string of the form "N1//N2" and parses the numbers.
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

Mesh* MeshParser::LoadFile(const std::string& path) {
  std::unique_ptr<Mesh> mesh(new Mesh());
  std::ifstream file_stream(path);

  if (!file_stream.is_open()) {
    return NULL;
  }

  while (!file_stream.eof()) {
    std::string type;
    file_stream >> type;

    if (type == kComment) {
      continue;
    } else if (type == kNormal) {
      Scalar x, y, z;
      file_stream >> x >> y >> z;
      mesh->AddNormal(Vector3(x, y, z));
    } else if (type == kVertex) {
      Scalar x, y, z;
      file_stream >> x >> y >> z;
      mesh->AddPoint(Point3(x, y, z));
    } else if (type == kTriangle) {
      size_t v1, v2, v3, n1, n2, n3;
      std::string index1_str, index2_str, index3_str;
      file_stream >> index1_str >> index2_str >> index3_str;

      ExtractIndex(index1_str, &v1, &n1);
      ExtractIndex(index2_str, &v2, &n2);
      ExtractIndex(index3_str, &v3, &n3);

      mesh->AddTriangle(v1-1, n1-1, v2-1, n2-1, v3-1, n3-1);
    }
  }
  return mesh.release();
}

// static
const std::string MeshParser::kComment = "#";

// static
const std::string MeshParser::kNormal = "vn";

// static
const std::string MeshParser::kVertex = "v";

// static
const std::string MeshParser::kTriangle = "f";
