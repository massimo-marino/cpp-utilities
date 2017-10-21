/* 
 * File:   utilities.h
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "concurrentLogging.h"
#include "containerHelpers.h"
#include "vectorHelpers.h"
#include "objectFactory.h"
#include "printHelpers.h"

#include <thread>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{

void yieldCPUAndSleep (const int64_t& nanoseconds = 0) noexcept;
void removeMultipleOccurrences(std::string& s) noexcept;

////////////////////////////////////////////////////////////////////////////////
// check if all the parameters are within a certain range
template <typename T, typename ... Ts>
bool within(const T& min, const T& max, const Ts& ...ts) noexcept
{
  return ((min <= ts && ts <= max) && ...);
}
////////////////////////////////////////////////////////////////////////////////
// a simple function's performance timer
template <typename Time = std::chrono::nanoseconds,
          typename Clock = std::chrono::high_resolution_clock>
struct perftimer
{
  template <typename F, typename... Args>
  static inline Time duration(F&& f, Args&&... args)
  {
    auto start = Clock::now();
    // C++17: not yet available
    //std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f(std::forward<Args>(args)...);
    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }
};
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities

////////////////////////////////////////////////////////////////////////////////
#endif /* UTILITIES_H */
