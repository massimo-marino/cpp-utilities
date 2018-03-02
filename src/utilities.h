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
#include "hasMember.h"
#include "perfTimer.h"
#include "printHelpers.h"
#include "randomNumberGenerators.h"
#include "stringTokenizers.h"
#include "tprintf.h"
#include "vectorHelpers.h"

#include <thread>
////////////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_LOG_MACROS
#define DEBUG_LOG_MACROS
// macros L and LL give the same result
#define L std::cout << "[" << __FILE__ << ":" << __LINE__ << ": " << __func__ << "]: "
#define LL [](const std::string& function) {std::cout << "[" << __FILE__ << ":" << __LINE__ << ": " << function << "]: ";}(__func__)
// macros LMSG and LLMSG give the same result
#define LMSG(msg) std::cout << "[" << __FILE__ << ":" << __LINE__ << ": " << __func__ << "]: " << msg << std::endl
#define LLMSG(msg) [](const std::string& function, const std::string& s) {std::cout << "[" << __FILE__ << ":" << __LINE__ << ": " << function << "]: "<< s  << std::endl;}(__func__, msg)
#else
#error "DEBUG_LOG_MACROS symbol already defined!"
#endif

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
