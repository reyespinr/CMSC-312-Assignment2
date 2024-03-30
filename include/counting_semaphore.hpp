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

private:
  int val;
  std::mutex mutex;
  std::condition_variable gate;
};

#endif  // COUNTING_SEMAPHORE_HPP
