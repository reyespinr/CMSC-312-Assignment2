#ifndef COUNTING_SEMAPHORE_HPP
#define COUNTING_SEMAPHORE_HPP

#include <condition_variable>
#include <mutex>

class CountingSemaphore
{
public:
  CountingSemaphore(int K);
  void P();
  void V();
  int get_val() const;
  void reset(int K);  // Declare the reset method

private:
  int val;
  mutable std::mutex mutex;  // Make mutex mutable
  std::condition_variable gate;
};

#endif  // COUNTING_SEMAPHORE_HPP
