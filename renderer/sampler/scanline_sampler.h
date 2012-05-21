/*
 * A sampler which simply samples the pixels in linear order.
 * Author: Dino Wernli
 */

#ifndef SCANLINE_SAMPLER_H_
#define SCANLINE_SAMPLER_H_

#include <mutex>

#include "renderer/sampler/sampler.h"
#include "util/no_copy_assign.h"

class Camera;
class Sample;

class ScanlineSampler : public Sampler {
 public:
  ScanlineSampler(bool thread_safe);
  virtual ~ScanlineSampler();
  NO_COPY_ASSIGN(ScanlineSampler);

  // Not made thread safe, expected to be called only once.
  virtual void Init(const Camera* camera);

  virtual size_t MaxJobSize() const { return kJobSize; }
  virtual size_t NextJob(std::vector<Sample>* samples);
  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n);

 private:
  // Helper method which fetches the next sample. Returns true if the new sample
  // was successfully stored in sample, and false if there are no samples left.
  bool InternalNextSample(Sample* sample);

  // Iteration variables for the scan line.
  size_t current_x_;
  size_t current_y_;

  static const size_t kJobSize;
};

#endif  /* SCANLINE_SAMPLER_H_ */
