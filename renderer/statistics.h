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
    LOG(INFO) << "Initlializing statistics";
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
