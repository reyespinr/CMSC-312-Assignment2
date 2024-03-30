// part3.cpp
#include <sys/wait.h>  // For wait()
#include <unistd.h>    // For fork()

#include <iostream>
#include <thread>
#include <vector>

#include "shared_data.hpp"

int main(int argc, char * argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <num_writers> <num_readers>\n";
    return 1;
  }

  int num_writers = std::stoi(argv[1]);
  int num_readers = std::stoi(argv[2]);

  SharedMemoryContent * shared_memory = initialize_shared_memory();

  // Create writer processes
  for (int i = 0; i < num_writers; ++i) {
    if (fork() == 0) {
      writer_process(shared_memory, i);
      exit(0);
    }
  }

  // Create reader threads
  std::vector<std::thread> readers;
  for (int i = 0; i < num_readers; ++i) {
    readers.emplace_back(reader_thread, shared_memory, i);
  }

  // Wait for all writers to finish
  for (int i = 0; i < num_writers; ++i) {
    wait(nullptr);
  }
  // Signal that all writers are finished
  shared_memory->allWritersFinished = true;

  // Post the semaphore to wake up any sleeping readers one last time
  for (int i = 0; i < num_readers; ++i) {
    sem_post(&shared_memory->readSem);
  }

  // Join all reader threads
  for (auto & reader : readers) {
    reader.join();
  }

  cleanup_shared_memory(shared_memory);

  return 0;
}
