#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/utils.hpp"
#include "reader_writer.hpp"

// Introduces a delay for testing concurrency aspects.
auto delay(int milliseconds) -> void
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Handles SIGINT (Ctrl+C) for graceful termination.
auto signal_handler(int signum) -> void
{
  std::cout << "Interrupt signal (" << signum << ") received. Exiting gracefully.\n";
  exit(signum);
}

// Tests the functionality of the writer to ensure it correctly modifies the shared resource.
auto test_writer_functionality() -> void
{
  ReaderWriter rw;
  int initial_count = rw.get_count();
  rw.writer(1);  // Execute one writer operation
  assertTest(rw.get_count() == 2 * initial_count, "Writer did not double the count correctly.");
}

// Tests that multiple readers can simultaneously read without violating the upper limit.
auto test_multiple_readers() -> void
{
  ReaderWriter rw;
  rw.test_mode = true;  // Enable delayed reading for testing concurrency
  std::thread reader1([&rw]() { rw.reader(1); });
  std::thread reader2([&rw]() { rw.reader(2); });
  delay(50);  // Short delay to ensure both readers have started
  assertTest(rw.get_num_reader() >= 2, "Multiple readers are not being counted correctly.");
  reader1.join();
  reader2.join();
  assertTest(rw.get_num_reader() == 0, "Reader count did not reset correctly.");
  std::cout << std::endl;  // Add newline after the readers finish
}

// Test to ensure writer exclusivity: no readers can read while a writer is writing.
auto test_writer_exclusivity() -> void
{
  ReaderWriter rw;
  rw.test_mode = true;  // Delay readers to help ensure writer starts first

  std::atomic<bool> writer_started{false};
  std::atomic<bool> reader_blocked{true};

  std::thread writer([&rw, &writer_started]() {
    rw.writer(1);
    writer_started = true;  // Writer indicates it has started
  });

  // Attempt to start reader shortly after writer
  delay(50);

  std::thread reader([&rw, &writer_started, &reader_blocked]() {
    if (!writer_started) {
      reader_blocked = false;  // If reader starts before writer, mark test to fail
    }
    rw.reader(1);
  });

  writer.join();
  reader.join();

  assertTest(reader_blocked, "Reader was not properly blocked by active writer.");
  std::cout << std::endl;  // Add newline after the readers finish
}

// Test to observe reader priority over writers when already reading.
auto test_reader_priority() -> void
{
  ReaderWriter rw;
  rw.test_mode = true;

  std::atomic<int> readers_count{0};
  std::atomic<bool> writer_waiting{false};

  std::thread reader1([&rw, &readers_count]() {
    rw.reader(1);
    readers_count++;
    delay(100);  // Simulate reading takes some time
    readers_count--;
  });

  delay(50);  // Wait to ensure the first reader starts before the writer

  std::thread writer([&rw, &writer_waiting, &readers_count]() {
    while (readers_count.load() == 0) {  // Wait until at least one reader is active
                                         // No operation, just waiting
    }
    writer_waiting = true;
    rw.writer(1);
    writer_waiting = false;
  });

  delay(50);  // Ensure the writer has started and is potentially waiting

  // Corrected capture for writer_waiting
  std::thread reader2([&rw, &readers_count, &writer_waiting]() {
    if (writer_waiting.load()) {  // Check if the writer is waiting
      rw.reader(2);
      readers_count++;
      delay(100);  // Simulate reading takes some time
      readers_count--;
    }
  });

  reader1.join();
  writer.join();
  reader2.join();

  assertTest(!writer_waiting.load(), "Writer did not wait for readers to finish.");
  assertTest(readers_count.load() == 0, "Not all readers finished correctly.");
  std::cout << std::endl;  // Add newline after the readers finish
}

// Stress test to observe behavior under a high load of readers and writers.
auto test_stress_conditions() -> void
{
  ReaderWriter rw;
  const int readers_num = 100;
  const int writers_num = 50;

  std::vector<std::thread> readers;
  std::vector<std::thread> writers;

  // Launch a large number of readers
  for (int i = 0; i < readers_num; ++i) {
    readers.emplace_back([&rw, i]() { rw.reader(i + 1); });
  }

  // Launch a large number of writers
  for (int i = 0; i < writers_num; ++i) {
    writers.emplace_back([&rw, i]() { rw.writer(i + 1); });
  }

  // Wait for all threads to complete
  for (auto & reader : readers) {
    reader.join();
  }
  for (auto & writer : writers) {
    writer.join();
  }

  // Output test completion message
  // std::cout << "Stress test completed with " << readers_num << " readers and " << writers_num
  //          << " writers." << std::endl;
  // std::cout << "test_stress_conditions passed." << std::endl;
  // std::cout << std::endl;  // Ensure clear separation between tests
}

// Main function to run defined tests
int main()
{
  std::signal(SIGINT, signal_handler);  // Setup signal handler for graceful exit

  RUN_TEST(test_writer_functionality);
  RUN_TEST(test_multiple_readers);
  RUN_TEST(test_writer_exclusivity);
  RUN_TEST(test_reader_priority);
  RUN_TEST(test_stress_conditions);

  return 0;
}
