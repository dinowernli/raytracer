/*
 * Unit tests for the Image class.
 * Author: Dino Wernli
 */

#include <gtest/gtest.h>

#include "renderer/image.h"

static bool ColorsEqual(Color3 c1, Color3 c2) {
  return (c1.r() == c2.r() && c1.g() == c2.g() && c1.b() == c2.b());
}

TEST(Image, ImageBlackInitially) {
  Image image(10, 20);

  EXPECT_EQ(10, image.SizeX());
  EXPECT_EQ(20, image.SizeY());

  for (size_t x = 0; x < image.SizeX(); ++x) {
    for (size_t y = 0; y < image.SizeY(); ++y) {
      EXPECT_TRUE(ColorsEqual(Color3(0, 0, 0), image.PixelAt(x, y)));
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
      EXPECT_TRUE(ColorsEqual(Color3(1, 0, 1), image.PixelAt(x, y)));
    }
  }
}
