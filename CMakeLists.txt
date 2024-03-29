cmake_minimum_required(VERSION 3.16)
project(CMSC-312-Assignment2)

set(CMAKE_CXX_STANDARD 20) # Ensure C++20 is used

include_directories(include)

# Executables for Q1
add_executable(ReaderWriter src/reader_writer.cpp)

# Link pthread library
find_package(Threads REQUIRED) # Find the threads package
target_link_libraries(ReaderWriter PRIVATE Threads::Threads) # Link against pthread
