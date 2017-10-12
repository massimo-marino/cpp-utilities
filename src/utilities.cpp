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
////////////////////////////////////////////////////////////////////////////////
// helpers for concurrent logging
pclog::~pclog()
{
  std::lock_guard<std::mutex> lg {clog_mutex};
  std::clog << rdbuf();
  std::clog.flush();
}

pcout::~pcout()
{
  std::lock_guard<std::mutex> lg {cout_mutex};
  std::cout << rdbuf();
  std::cout.flush();
}

pcerr::~pcerr()
{
  std::lock_guard<std::mutex> lg {cerr_mutex};
  std::cerr << rdbuf();
  std::cerr.flush();
}
////////////////////////////////////////////////////////////////////////////////

void yieldCPUAndSleep (const int64_t& nanoseconds) noexcept
{
  std::this_thread::yield();
  if ( 0 == nanoseconds )
  {
    return;
  }
  std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
}

void removeMultipleOccurrences(std::string& s) noexcept
{
  s.erase(std::unique(std::begin(s), std::end(s)), end(s));
}

}  // namespace utilities
