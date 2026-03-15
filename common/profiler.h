#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <iostream>

/**
 * @brief Get the execution time of a block of code.
 */
#define PROFILE_BLOCK(x)                                                                \
    auto start = std::chrono::high_resolution_clock::now();                             \
    x auto end = std::chrono::high_resolution_clock::now();                             \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

#endif
