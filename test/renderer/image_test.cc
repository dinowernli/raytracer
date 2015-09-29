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
 * Unit tests for the Image class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "renderer/image.h"
#include "test/test_util.h"

namespace {

TEST(Image, ImageBlackInitially) {
  Image image(10, 20);

  EXPECT_EQ(10, image.SizeX());
  EXPECT_EQ(20, image.SizeY());

  for (size_t x = 0; x < image.SizeX(); ++x) {
    for (size_t y = 0; y < image.SizeY(); ++y) {
      EXPECT_TRUE(TestUtil::ColorsEqual(Color3(0, 0, 0), image.PixelAt(x, y)));
    }
  }
}

TEST(Image, ColorStoredAndRetrieved) {
  Image image(10, 20);

  for (size_t x = 0; x < image.SizeX(); ++x) {
    for (size_t y = 0; y < image.SizeY(); ++y) {
      image.PutPixel(Color3(1, 0, 1), x, y);
    }
  }

  for (size_t x = 0; x < image.SizeX(); ++x) {
    for (size_t y = 0; y < image.SizeY(); ++y) {
      EXPECT_TRUE(TestUtil::ColorsEqual(Color3(1, 0, 1), image.PixelAt(x, y)));
    }
  }
}

TEST(Image, EmptyImage) {
  Image image(0, 0);
  EXPECT_EQ(0, image.SizeX());
  EXPECT_EQ(0, image.SizeY());
}

}  // namespace
