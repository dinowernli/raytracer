/*
 * A sampler which progressively fills out the picture.
 * Author: Dino Wernli
 */

#ifndef PROGRESSIVE_SAMPLER_H_
#define PROGRESSIVE_SAMPLER_H_

#include <algorithm>

#include "renderer/sampler/sampler.h"
#include "util/no_copy_assign.h"

class ProgressiveSampler : public Sampler {
 public:
  ProgressiveSampler(bool thread_safe);
  virtual ~ProgressiveSampler();
  NO_COPY_ASSIGN(ProgressiveSampler);

  // Not made thread safe, expected to be called only once.
  virtual void Init(const Camera* camera);

  virtual size_t MaxJobSize() const { return kJobSize; }
  virtual size_t NextJob(std::vector<Sample>* samples);
  virtual void AcceptJob(const std::vector<Sample>& samples, size_t n);
  virtual bool IsThreadSafe() const { return thread_safe_; }

 private:
  // Helper method which fetches the next sample. Returns true if the new sample
  // was successfully stored in sample, and false if there are no samples left.
  bool InternalNextSample(Sample* sample);

  // Iteration variables.
  size_t current_x_;
  size_t current_y_;
  size_t current_size_x_;
  size_t current_size_y_;

  // Indicates whether or not to expect multiple threads to interact with this
  // sampler.
  bool thread_safe_;

  // Lock used for synchronizing access to the critical methods.
  std::mutex lock_;

  // TODO(dinow): Increase this over time.
  static const size_t kJobSize;
};

#endif  /* PROGRESSIVE_SAMPLER_H_ */
