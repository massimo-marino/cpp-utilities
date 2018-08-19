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

bool
isLittleEndian() noexcept
{
  constexpr static auto v {0x0001};
  const static unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};

  return p[0] == static_cast<unsigned char>(0x01);
}

bool
isBigEndian() noexcept
{
  constexpr static auto v {0x0001};
  const static unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};

  return p[0] == static_cast<unsigned char>(0x00);
}

}  // namespace utilities
