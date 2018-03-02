//
// tprintf.ccp
//
// Created by massimo on 2/16/18.
//

#include "tprintf.h"
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// tprintf: templated printf: base function for a generic std::ostream
std::ostream&
tprintf(std::ostream& os, const char* format) noexcept
{
  return os << format;
}

// tprintf: templated printf: base function for std::cout
std::ostream&
tprintf(const char* format) noexcept
{
  return std::cout << format;
}

// tprintf: templated printf: base function for a generic std::ostream
std::ostream&
tprintf(std::ostream& os, const std::string& format) noexcept
{
  return os << format;
}

// tprintf: templated printf: base function for std::cout
std::ostream&
tprintf(const std::string& format) noexcept
{
  return std::cout << format;
}
}  // namespace utilities
