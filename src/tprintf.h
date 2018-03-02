//
// tprintf.h
//
// Created by massimo on 2/16/18.
//

#pragma once

#include <iostream>
#include <sstream>
////////////////////////////////////////////////////////////////////////////////
namespace utilities {
// tprintf: templated printf for a generic std::ostream
// base function
std::ostream &
tprintf(std::ostream &os, const char *format) noexcept;

// tprintf: recursive variadic function
template<typename T, typename... Targs>
std::ostream &
tprintf(std::ostream &os, const char *format, T value, const Targs... Fargs) noexcept {
  for (; '\0' != *format; ++format) {
    if ('%' == *format) {
      os << value;
      // recursive call
      return tprintf(os, format + 1, Fargs...);
    }
    os << *format;
  }
  return os;
}

// tprintf: templated printf for std::cout
// base function
std::ostream &
tprintf(const char *format) noexcept;

// tprintf: recursive variadic function
template<typename T, typename... Targs>
std::ostream &
tprintf(const char *format, T value, const Targs... Fargs) noexcept {
  return tprintf(std::cout, format, value, Fargs...);
}

// tprintf: templated printf for a generic std::ostream
// base function
std::ostream &
tprintf(std::ostream &os, const std::string &format) noexcept;

template<typename T, typename... Targs>
std::ostream &
tprintf(std::ostream &os, const std::string &format, T value, const Targs... Fargs) noexcept {
  return tprintf(os, format.data(), value, Fargs...);
}

std::ostream &
tprintf(const std::string &format) noexcept;

// tprintf: recursive variadic function
template<typename T, typename... Targs>
std::ostream &
tprintf(const std::string &format, T value, const Targs... Fargs) noexcept {
  return tprintf(std::cout, format.data(), value, Fargs...);
}
}  // namespace utlities
