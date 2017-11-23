/* 
 * File:   printHelpers.cpp
 * Author: massimo
 *
 * Created on October 13, 2017, 8:36 AM
 */
#include "printHelpers.h"
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// tprintf: templated printf
void
tprintf(std::ostream& os, const char* format) noexcept // base function
{
  os << format;
}
}  // namespace utilities
