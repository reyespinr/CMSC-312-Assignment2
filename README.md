# CMSC-312 Assignment 2

This repository contains a collection of programs that demonstrate different inter-process communication (IPC) methods and multi-threading in C++, focusing on shared memory, semaphores, and process-thread synchronization.

## Structure

- `include/`: Header files for shared structures and utilities.
- `src/`: Source files for all IPC examples, including client-server socket programs and message queue examples.

## Building and Running

### Prerequisites

- CMake (version 3.16 or higher)
- A C++ compiler supporting C++20 or higher (e.g., g++ or clang)
- VSCode (optional, with CMake Tools extension for easy building and running)

### Using VSCode Extension

1. Open the project folder in VSCode.
2. Ensure CMake Tools extension is installed.
3. Configure the project by selecting a kit (compiler).
4. Build the project by clicking the build button (✓) in the bottom blue bar.
5. Run any executable by clicking on the play button next to the build button.

### Using Command Line

In the project root directory, run the following commands:

```bash
mkdir build && cd build      # Create and enter the build directory
cmake ..                     # Generate build system files
cmake --build .              # Compile the project
```

## Running Executables
After building, executables will be located in the build directory. They can be run with commands like:

```bash
./Part1   # For part 1 executable
./Part2   # For part 2 executable
./Part3   # For part 3 executable, use with arguments, e.g., ./Part3 5 10
```

## Programs by Part

### Part 1: Reader-Writer Problem Using Shared Memory

- Demonstrates the reader-writer problem using counting semaphores to synchronize access to shared memory.

### Part 2: Counting Semaphores with Binary Semaphores

- Implements counting semaphores using binary semaphores and tests them with a custom reader-writer problem.

### Part 3: Multi-Process and Multi-Threaded Implementation

- Converts writer logic to separate processes and maintains readers as threads, all synchronized using shared memory and semaphores.
