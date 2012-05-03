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
  ScanlineSampler(bool thread_safe = false);
  virtual ~ScanlineSampler();
  NO_COPY_ASSIGN(ScanlineSampler);

  // Not made thread safe, expected to be called only once.
  virtual void Init(const Camera* camera);

  virtual bool NextSample(Sample* sample);

  virtual size_t MaxJobSize() const { return kJobSize; }

  virtual size_t NextJob(std::vector<Sample>* samples);

  virtual void AcceptSample(const Sample& sample);

  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n);

  virtual bool IsThreadSafe() const { return thread_safe_; }

  virtual double Progress() const;

 private:
  // Non-thread-safe version of NextSample which is used to fetch entire jobs.
  bool InternalNextSample(Sample* sample);

  // Non-thread-safe version of AcceptSample.
  void InternalAcceptSample(const Sample& sample);

  // Iteration variables for the scan line.
  size_t current_x_;
  size_t current_y_;

  // Size of the image.
  size_t width_;
  size_t height_;

  // The number of samples which have been returned with a color.
  size_t accepted_;

  // Indicates whether or not to expect multiple threads to interact with this
  // sampler.
  bool thread_safe_;

  // Lock used for synchronizing access to the critical methods.
  std::mutex lock_;

  static const size_t kJobSize;
};

#endif  /* SCANLINE_SAMPLER_H_ */
