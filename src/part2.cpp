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

  // Test case 1: Attempt to acquire semaphore more times than its capacity without immediate release.
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Test Case 1: Acquire semaphore more times than its capacity.\n";
  }
  std::vector<std::thread> test_case_1_threads;
  for (int i = 0; i < 5; ++i) {
    test_case_1_threads.emplace_back([&cs, i]() {
      cs.P();
      std::lock_guard<std::mutex> lock(cout_mutex);
      std::cout << "Semaphore acquired by thread " << i + 1
                << ". Remaining capacity: " << cs.get_val() << std::endl;
      // Intentionally not releasing the semaphore here to test exceeding capacity
    });
  }
  // Wait for threads to finish
  for (auto & t : test_case_1_threads) {
    t.join();
  }

  // Reset semaphore for next test case (not part of your original requirements, added for continuity)
  cs.reset(3);  // Assuming you add a reset method to restore the semaphore to its initial state

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

  // Test case 3: Acquire, release, and acquire semaphore again
  // Test case 3: Acquire, release, and acquire semaphore again
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "\nTest Case 3: Acquire, release, and acquire semaphore again.\n";
  }
  cs.P();
  std::lock_guard<std::mutex> lock(cout_mutex);
  std::cout << "Semaphore acquired. Remaining capacity: " << cs.get_val() << std::endl;
  cs.V();
  std::cout << "Semaphore released. Remaining capacity: " << cs.get_val() << std::endl;

  // Start a separate thread to acquire the semaphore after a delay
  std::thread([&cs]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cs.P();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Semaphore acquired again by another thread. Remaining capacity: " << cs.get_val()
              << std::endl;
  }).join();  // Wait for the thread to finish

  // Test case 4: Acquire semaphore in one thread while another thread is releasing it
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout
      << "\nTest Case 4: Acquire semaphore in one thread while another thread is releasing it.\n";
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

  // Test case 5: Acquire semaphore in multiple threads simultaneously
  {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "\nTest Case 5: Acquire semaphore in multiple threads simultaneously.\n";
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
