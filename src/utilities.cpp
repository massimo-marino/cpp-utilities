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
  constexpr auto v {0x01234567};
  const unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};

  return p[0] == static_cast<unsigned char>(0x67);
}

bool
isBigEndian() noexcept
{
  constexpr auto v {0x01234567};
  const unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};

  return p[0] == static_cast<unsigned char>(0x01);
}

unsigned int
replaceByte (unsigned int&& x,
             const int i,
             const unsigned char b) noexcept(false)
{
  const int W = sizeof(x);
  // 0 <= i <= W-1
  if ( (i >= W) || (i < 0) )
  {
    std::cerr << "replaceByte: ERROR: arg i: "
              << i
              << " but must be >= 0 and <= "
              << W - 1
              << std::endl;
    throw 1;
  }

  //unsigned char* p = reinterpret_cast<unsigned char*>(&x);
  //p[i] = b;
  reinterpret_cast<unsigned char*>(&x)[i] = b;

  return x;
}

}  // namespace utilities
