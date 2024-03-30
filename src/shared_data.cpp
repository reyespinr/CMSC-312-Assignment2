// shared_data.cpp
#include "shared_data.hpp"

#include <fcntl.h>     // For O_* constants
#include <sys/mman.h>  // For shared memory
#include <unistd.h>    // For ftruncate

#include <iostream>

SharedMemoryContent * initialize_shared_memory()
{
  // Open shared memory object
  int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("In shm_open()");
    exit(1);
  }
  ftruncate(shm_fd, sizeof(SharedMemoryContent));

  // Map shared memory in process space
  void * mapped_mem =
    mmap(0, sizeof(SharedMemoryContent), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (mapped_mem == MAP_FAILED) {
    perror("In mmap()");
    exit(1);
  }

  // Initialize semaphores and shared content
  SharedMemoryContent * shared_content = static_cast<SharedMemoryContent *>(mapped_mem);
  sem_init(
    &shared_content->writeSem, 1, 1);  // Binary semaphore for write access, initially available
  sem_init(
    &shared_content->readSem, 1, 0);  // Binary semaphore for read access, initially unavailable
  shared_content->allWritersFinished = false;

  return shared_content;
}

void cleanup_shared_memory(SharedMemoryContent * shared_memory)
{
  // Destroy semaphores
  sem_destroy(&shared_memory->writeSem);
  sem_destroy(&shared_memory->readSem);

  // Unmap and unlink the shared memory
  munmap(shared_memory, sizeof(SharedMemoryContent));
  shm_unlink("/my_shared_memory");
}

void writer_process(SharedMemoryContent * shared_memory, int writer_id)
{
  // Write to shared memory
  sem_wait(&shared_memory->writeSem);
  snprintf(
    shared_memory->str, sizeof(shared_memory->str),
    "Writer %d: The quick brown fox jumps over the lazy dog", writer_id);
  sem_post(&shared_memory->readSem);
}

void reader_thread(SharedMemoryContent * shared_memory, int reader_id)
{
  while (true) {
    sem_wait(&shared_memory->readSem);
    if (shared_memory->allWritersFinished) break;
    std::cout << "Reader " << reader_id << " sees: " << shared_memory->str << std::endl;
    sem_post(&shared_memory->writeSem);
  }
}
