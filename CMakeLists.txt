cmake_minimum_required(VERSION 3.16)
project(CMSC-312-Assignment2)

set(CMAKE_CXX_STANDARD 20) # Ensure C++20 is used

include_directories(include)

# Executable for part 1 of the assignment
add_executable(Part1 src/part1.cpp src/reader_writer.cpp)

# Executable for part 2 of the assignment
add_executable(Part2 src/part2.cpp src/counting_semaphore.cpp)

# Executable for part 3 of the assignment
add_executable(Part3 src/part3.cpp src/shared_data.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Part1 PRIVATE Threads::Threads)
target_link_libraries(Part2 PRIVATE Threads::Threads)
target_include_directories(Part3 PRIVATE include)
target_link_libraries(Part3 rt pthread)
