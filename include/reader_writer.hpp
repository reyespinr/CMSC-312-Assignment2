#ifndef READER_WRITER_HPP
#define READER_WRITER_HPP

#include <atomic>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

class ReaderWriter
{
public:
  explicit ReaderWriter(int max_readers = 5);
  ~ReaderWriter() = default;

  void writer(int writer_number);
  void reader(int reader_number);
  void run_simulation();

  int get_count() const;
  int get_num_reader() const;

  bool test_mode = false;  // Flag for test mode to delay reader execution

private:
  std::mutex mutex_;
  std::counting_semaphore<1> write_semaphore_;
  std::counting_semaphore<5> read_limit_;
  std::atomic<long long> count_;  // Changed to atomic<long long> to handle large numbers
  int num_reader_;
  std::mutex print_mutex;  // Mutex for printing
};

#endif  // READER_WRITER_HPP
