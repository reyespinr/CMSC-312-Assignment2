#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "counting_semaphore.hpp"

// Define a mutex for thread-safe cout
std::mutex cout_mutex;

void test_counting_semaphore()
{
  // Initialize counting semaphore with a capacity of 3
  CountingSemaphore cs(3);

  // Test case 1: Acquire semaphore more times than its capacity
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Test Case 1: Acquire semaphore more times than its capacity.\n";
  }
  for (int i = 0; i < 5; ++i) {
    cs.P();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Semaphore acquired. Remaining capacity: " << cs.get_val() << std::endl;
    cs.V();  // Release the semaphore after acquiring
  }
  // Test case 2: Acquire and release semaphore multiple times
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "\nTest Case 2: Acquire and release semaphore multiple times.\n";
  }
  for (int i = 0; i < 3; ++i) {
    cs.P();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Semaphore acquired. Remaining capacity: " << cs.get_val() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cs.V();
    std::cout << "Semaphore released. Remaining capacity: " << cs.get_val() << std::endl;
  }

  // Test case 3: Acquire semaphore in one thread while another thread is releasing it
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout
      << "\nTest case 3: Acquire semaphore in one thread while another thread is releasing it.\n";
  }
  std::thread t([&cs]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    cs.V();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Semaphore released by another thread.\n";
  });
  cs.P();
  t.join();
  std::cout << "Semaphore acquired. Remaining capacity: " << cs.get_val() << std::endl;

  // Test case 4: Acquire semaphore in multiple threads simultaneously
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "\nTest case 4: Acquire semaphore in multiple threads simultaneously.\n";
  }
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) {
    threads.emplace_back([&cs, i]() {
      cs.P();
      std::lock_guard<std::mutex> lock(cout_mutex);
      std::cout << "Thread " << i + 1 << " acquired semaphore. Remaining capacity: " << cs.get_val()
                << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      cs.V();
      std::cout << "Thread " << i + 1 << " released semaphore.\n";
    });
  }
  for (auto & thread : threads) {
    thread.join();
  }
}

int main()
{
  test_counting_semaphore();
  return 0;
}
