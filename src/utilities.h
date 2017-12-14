/* 
 * File:   utilities.h
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */
#pragma once

#include "cfile_helper.h"
#include "concurrentLogging.h"
#include "containerHelpers.h"
#include "vectorHelpers.h"
#include "printHelpers.h"
#include "stringTokenizers.h"
#include "randomNumberGenerators.h"
#include "hasMember.h"
#include "perfTimer.h"

#include <thread>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{

void yieldCPUAndSleep (const int64_t& nanoseconds = 0) noexcept;
void removeMultipleOccurrences(std::string& s) noexcept;

////////////////////////////////////////////////////////////////////////////////
// check if all the parameters are within a certain range
template <typename T, typename ... Ts>
bool
within(const T& min, const T& max, const Ts& ...ts) noexcept
{
  return ((min <= ts && ts <= max) && ...);
}
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
