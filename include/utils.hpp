// utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>
#include <iostream>
#include <string>

inline void assertTest(bool condition, const std::string & message)
{
  if (!condition) {
    std::cerr << "Test failed: " << message << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

#define RUN_TEST(testFunc)                                      \
  do {                                                          \
    std::cout << "Running " << #testFunc << "..." << std::endl; \
    testFunc();                                                 \
    std::cout << #testFunc << " passed." << std::endl;          \
  } while (0)

#endif  // UTILS_HPP
