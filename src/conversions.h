//
// conversions.h
//
// Created by massimo on 7/23/18.
//
#pragma once

#include <string>
#include <type_traits>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
template <typename numType>
std::string
numTypeToString(const numType &n) noexcept;

template <typename numType>
std::string
numTypeToString(const numType &n) noexcept
{
  static_assert(std::is_arithmetic<numType>::value,
                "ERROR: numTypeToString(numType) needs an arithmetic type as argument");

  return std::to_string(n);
}  // numTypeToString

template <typename numType>
numType
stringToNumType(const std::string &text);

template <typename numType>
numType
stringToNumType(const std::string &text)
{
  static_assert(std::is_arithmetic<numType>::value,
                "ERROR: stringToNumType<numType>() needs an arithmetic type as template argument");

  numType n {};
  std::stringstream ss(text);
  ss >> n;

  return n;
}  // stringToNumType

template <typename numType>
numType
stringToNumtype(const std::string &text, numType &n);

template <typename numType>
numType
stringToNumtype(const std::string &text, numType &n)
{
  static_assert(std::is_arithmetic<numType>::value,
                "ERROR: stringToNumtype<numType>() needs an arithmetic type as template argument");

  n = stringToNumType<numType>(text);

  return n;
}  // str_to_numtype
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
