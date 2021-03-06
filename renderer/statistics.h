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
 * A class which can keep track of all sorts of rendering stats.
 * Author: Dino Wernli
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <glog/logging.h>
#include <memory>

#include "listener/bmp_exporter.h"
#include "renderer/image.h"

class Statistics {
 public:
  // Takes ownership of the heatmap exporter.
  Statistics(BmpExporter* sampling_heatmap_exporter) {
    sampling_heatmap_exporter_.reset(sampling_heatmap_exporter);
  }

  void Init(size_t width, size_t height) {
    LOG(INFO) << "Initializing statistics";
    sampling_heatmap_.reset(new Image(width, height));
  }

  void Export() const {
    // Sampler just stored number of rays per pixel. Normalize.
    Intensity maximum = sampling_heatmap_->MaxIntensity();
    if (maximum != 0) {
      sampling_heatmap_->Scale(1.0 / maximum);
    }
    sampling_heatmap_exporter_->Export(*sampling_heatmap_);
  }

  Image* sampling_heatmap() const { return sampling_heatmap_.get(); }

 private:
  std::unique_ptr<BmpExporter> sampling_heatmap_exporter_;
  std::unique_ptr<Image> sampling_heatmap_;
};

#endif  /* STATISTICS_H_ */
