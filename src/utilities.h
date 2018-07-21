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

#include <iomanip>
#include <thread>
#include <cxxabi.h>
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

template <typename T>
std::string
getDemangledTypeName() noexcept;

template <typename T>
std::string
getDemangledTypeName() noexcept
{
  int   status{};
  char* demangledName {abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status)};
  std::string demangledNameString {static_cast<std::string>(demangledName)};

  free(demangledName);
  return demangledNameString;
  //return static_cast<std::string>(typeid(T).name());
}

// FP1, FP2 := float, double, long double
template <typename FP1, typename FP2>
auto
fpEqual(FP1 arg1,
        FP2 arg2,
        long double E = static_cast<long double>(1.e-20),
        bool verbose = false) noexcept -> bool;

// see: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
// this code implements an easy and dirty solution
template<typename FP1, typename FP2>
auto
fpEqual(const FP1 arg1,
        const FP2 arg2,
        const long double E,
        const bool verbose) noexcept -> bool
{
  // compile-time type check
  static_assert(std::is_floating_point<FP1>::value, "ERROR: fpEqual() needs floating points arguments but arg1 is not");
  static_assert(std::is_floating_point<FP2>::value, "ERROR: fpEqual() needs floating points arguments but arg2 is not");
  static_assert(std::is_same<FP1, FP2>::value, "ERROR: fpEqual() needs two floating point arguments of the same type");

  // display the type names
  if ( verbose )
  {
    const auto tn1 = getDemangledTypeName<FP1>();
    const auto tn2 = getDemangledTypeName<FP2>();
    std::cerr << "arg1 type: "
              << tn1
              << "\narg2 type: "
              << tn2
              << "\n"
              << std::setprecision(36)
              << "arg1: "
              << arg1
              << "\narg2: "
              << arg2
              << "\nfabsl(arg1 - arg2): "
              << fabsl(static_cast<long double>(arg1) - static_cast<long double>(arg2))
              << "\n";
  }

  // use fabsl() NOT abs()
  return fabsl(static_cast<long double>(arg1) - static_cast<long double>(arg2)) < E;
}

template <typename T>
T
replaceByte(T&& x,
            size_t i,
            unsigned char b) noexcept(false);

template <typename T>
T
replaceByte(T&& x,
            const size_t i,
            const unsigned char b) noexcept(false)
{
  auto W = sizeof(T);
  // 0 <= i <= W-1
  if ( i >= W )
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

bool isLittleEndian () noexcept;
bool isBigEndian () noexcept;

////////////////////////////////////////////////////////////////////////////////
// check if all the parameters are within a certain range
template <typename T, typename ... Ts>
bool
within(const T& min, const T& max, const Ts& ...ts) noexcept;

template<typename T, typename... Ts>
bool
within(const T &min, const T &max, const Ts &... ts) noexcept
{
  return ((min <= ts && ts <= max) && ...);
}
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
