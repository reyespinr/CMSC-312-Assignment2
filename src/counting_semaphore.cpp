#include "counting_semaphore.hpp"

CountingSemaphore::CountingSemaphore(int K) : val(K) {}

void CountingSemaphore::P()
{
  std::unique_lock<std::mutex> lock(mutex);
  gate.wait(lock, [this] { return val > 0; });
  val--;
}

void CountingSemaphore::V()
{
  {
    std::lock_guard<std::mutex> lock(mutex);
    val++;
  }
  gate.notify_one();
}

int CountingSemaphore::get_val() const
{
  std::lock_guard<std::mutex> lock(mutex);
  return val;
}

// Implement a reset method if needed
void CountingSemaphore::reset(int K) {
  std::lock_guard<std::mutex> lock(mutex);
  val = K;
  gate.notify_all(); // Notify all, in case they are waiting, to re-check the condition.
}
