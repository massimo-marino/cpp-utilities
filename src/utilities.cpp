/* 
 * File:   utilities.cpp
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#include "utilities.h"
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
void
yieldCPUAndSleep (const int64_t& nanoseconds) noexcept
{
  std::this_thread::yield();
  if ( 0 == nanoseconds )
  {
    return;
  }
  std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
}

void
removeMultipleOccurrences(std::string& s) noexcept
{
  s.erase(std::unique(std::begin(s), std::end(s)), end(s));
}
}  // namespace utilities
