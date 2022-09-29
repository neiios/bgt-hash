#pragma once

/**
 * @file timer.h
 * Timer class.
 */

#include <chrono>

/**
 * @brief Timer class for measuring time used by functions.
 *
 */
class Timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;

 public:
  /**
   * @brief Construct a new Timer object
   *
   */
  Timer() : start{std::chrono::high_resolution_clock::now()} {}
  /**
   * @brief Reset timer
   *
   */
  void reset() { start = std::chrono::high_resolution_clock::now(); }
  /**
   * @brief Return time in seconds
   *
   * @return double
   */
  double elapsed() const {
    return std::chrono::duration<double>(
               std::chrono::high_resolution_clock::now() - start)
        .count();
  }
};
