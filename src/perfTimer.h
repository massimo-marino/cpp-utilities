/* 
 * File:   perfTimer.h
 * Author: massimo
 *
 * Created on December 6, 2017, 11:32 AM
 */
#pragma once

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
    // C++17: not yet available
    //invoke(f, (args) ...);
    //invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f(std::forward<Args>(args)...);
    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }
};
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
