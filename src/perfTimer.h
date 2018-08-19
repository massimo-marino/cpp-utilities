/* 
 * File:   perfTimer.h
 * Author: massimo
 *
 * Created on December 6, 2017, 11:32 AM
 */
#pragma once

#include <ctime>
#include <ratio>
#include <chrono>
#include <functional>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// a simple function's performance timer
template <typename Time = std::chrono::nanoseconds,
          typename Clock = std::chrono::high_resolution_clock>
struct perftimer
{
  template <typename F, typename... Args>
  static inline
  Time
  duration(F&& f, Args&&... args)
  {
    auto start = Clock::now();

    // C++17
    //invoke(f, (args) ...);
    //invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f(std::forward<Args>(args)...);

    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }  // duration

  template <typename F, typename... Args>
  static inline
  double
  duration_seconds(F&& f, Args&&... args)
  {
    typename Clock::time_point start = Clock::now();

    // C++17
    //invoke(f, (args) ...);
    //invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f(std::forward<Args>(args)...);

    typename Clock::time_point end = Clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    return time_span.count();
  }  // duration_seconds
};  // struct perftimer
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
