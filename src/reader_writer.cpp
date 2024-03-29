#include <iostream>
#include <mutex>
#include <semaphore>  // For std::counting_semaphore
#include <thread>
#include <vector>

std::mutex mutex;
std::counting_semaphore<1> wrt(1);
std::counting_semaphore<5> readLimit(5);  // Directly initialize with N
int cnt = 1;
int numreader = 0;

auto writer(int wno) -> void
{
  wrt.acquire();
  cnt = cnt * 2;
  std::cout << "Writer " << wno << " modified cnt to " << cnt << std::endl;
  wrt.release();
}

auto reader(int rno) -> void
{
  readLimit.acquire();  // Limit the number of readers
  std::unique_lock<std::mutex> lock(mutex);
  numreader++;
  if (numreader == 1) {
    wrt.acquire();  // If this is the first reader, block the writer
  }
  lock.unlock();

  // Reading section
  std::cout << "Reader " << rno << ": read cnt as " << cnt << std::endl;

  lock.lock();
  numreader--;
  if (numreader == 0) {
    wrt.release();  // If this is the last reader, wake up the writer
  }
  lock.unlock();
  readLimit.release();  // Indicate reader is done
}

auto main() -> int
{
  std::vector<std::thread> readers;
  std::vector<std::thread> writers;

  // Create threads
  for (int i = 0; i < 10; i++) {
    readers.emplace_back(reader, i + 1);
  }
  for (int i = 0; i < 5; i++) {
    writers.emplace_back(writer, i + 1);
  }

  // Join threads
  for (auto & th : readers) th.join();
  for (auto & th : writers) th.join();

  // No need to explicitly destroy semaphores as they are managed by C++ destructors

  return 0;
}
