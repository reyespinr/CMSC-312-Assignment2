#include "reader_writer.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <limits>
#include <mutex>  // Add mutex header

ReaderWriter::ReaderWriter(int max_readers)
: write_semaphore_(1), read_limit_(max_readers), count_(1), num_reader_(0)
{
}

auto ReaderWriter::writer(int writer_number) -> void
{
  write_semaphore_.acquire();
  // Temporary solution to safely double the count, considering atomicity
  long long current = count_.load();  // Load current value
  if (current <= (std::numeric_limits<long long>::max() / 2)) {
    // We can safely double the count without risking overflow
    count_.store(current * 2);                      // Store the updated value
    std::lock_guard<std::mutex> lock(print_mutex);  // Lock the mutex
    std::cout << "Writer " << writer_number << " modified count to " << count_.load() << std::endl;
  } else {
    std::lock_guard<std::mutex> lock(print_mutex);  // Lock the mutex
    std::cout << "Writer " << writer_number << ": overflow risk, count not doubled." << std::endl;
  }
  write_semaphore_.release();
}

void ReaderWriter::reader(int reader_number)
{
  read_limit_.acquire();
  std::unique_lock<std::mutex> lock(mutex_);
  num_reader_++;
  if (num_reader_ == 1) {
    write_semaphore_.acquire();
  }
  lock.unlock();

  {
    std::lock_guard<std::mutex> lock(print_mutex);  // Lock the mutex
    std::cout << "Reader " << reader_number << ": read count as " << count_.load() << std::endl;
  }

  if (test_mode) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  lock.lock();
  num_reader_--;
  if (num_reader_ == 0) {
    write_semaphore_.release();
  }
  lock.unlock();
  read_limit_.release();
}

void ReaderWriter::run_simulation()
{
  std::vector<std::thread> readers;
  std::vector<std::thread> writers;

  for (int i = 0; i < 10; i++) {
    readers.emplace_back(&ReaderWriter::reader, this, i + 1);
  }
  for (int i = 0; i < 5; i++) {
    writers.emplace_back(&ReaderWriter::writer, this, i + 1);
  }

  for (auto & th : readers) th.join();
  for (auto & th : writers) th.join();
}

int ReaderWriter::get_count() const
{
  return count_.load();  // Using .load() for atomic<long long>
}

int ReaderWriter::get_num_reader() const { return num_reader_; }
