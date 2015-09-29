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

#ifndef MESH_PARSER_H_
#define MESH_PARSER_H_

#include<string>
#include<vector>

#include "util/no_copy_assign.h"
#include "util/point3.h"
#include "util/vector3.h"

class Mesh;

class MeshParser {
 public:
  MeshParser();
  virtual ~MeshParser();
  NO_COPY_ASSIGN(MeshParser);

  // Parses the file and returns a new mesh with the contents, or NULL if the
  // mesh could not be loaded. The caller takes ownership of the returned
  // pointer.
  Mesh* LoadFile(const std::string& path);

  static const std::string kComment;
  static const std::string kNormal;
  static const std::string kVertex;
  static const std::string kTriangle;
};

#endif  /* MESH_PARSER_H_ */
