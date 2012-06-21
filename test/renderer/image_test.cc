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
