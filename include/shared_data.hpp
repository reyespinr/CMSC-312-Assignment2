// shared_data.hpp
#ifndef SHARED_DATA_HPP
#define SHARED_DATA_HPP

#include <semaphore.h>

struct SharedMemoryContent
{
  char str[256];  // Shared string buffer
  sem_t writeSem;
  sem_t readSem;
  bool allWritersFinished;  // Used to signal readers to stop
};

// Functions to manage shared resources
SharedMemoryContent * initialize_shared_memory();
void cleanup_shared_memory(SharedMemoryContent * shared_memory);
void writer_process(SharedMemoryContent * shared_memory, int writer_id);
void reader_thread(SharedMemoryContent * shared_memory, int reader_id);

#endif  // SHARED_DATA_HPP
