/* 
 * File:   printHelpers.h
 * Author: massimo
 *
 * Created on October 13, 2017, 8:36 AM
 */
#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <map>
#include <vector>
#include <valarray>
#include <deque>
#include <iterator>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
template <typename T>
void
showBytes (T&& v, std::ostream &os = std::cerr) noexcept;

template <typename T>
void
showBytes (T&& v, std::ostream &os) noexcept
{
  const unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};
  const auto W {sizeof(v)};
  unsigned char item {};

  for (int i {0}; i < W; ++i)
  {
    item = p[i];
    os << static_cast<const void*>(&(p[i]))
       << "["
       << std::dec
       << i
       << "]: "
       << std::setw(3)
       << std::setfill(' ')
       << static_cast<unsigned short>(item)
       << " [0x"
       << std::hex
       << std::setw(2)
       << std::setfill('0')
       << static_cast<unsigned short>(item)
       << "]\n";
  }
  os << std::endl;
}

// see: https://en.cppreference.com/w/cpp/utility/bitset
template <typename T>
void
showBits (T&& v, std::ostream &os = std::cerr) noexcept;

template <typename T>
void
showBits (T&& v, std::ostream &os) noexcept
{
  const unsigned char* p {reinterpret_cast<const unsigned char*>(&v)};
  const auto W {sizeof(v)};
  const auto CHAR_SIZE {8};
  unsigned char item {};

  for (int i {0}; i < W; ++i)
  {
    item = p[i];
    os << static_cast<const void *>(&(p[i]))
       << "["
       << std::dec
       << i
       << "]: "
       << std::setw(CHAR_SIZE)
       << std::setfill('0')
       << std::bitset<CHAR_SIZE>(item)
       << " [0x"
       << std::hex
       << std::setw(2)
       << std::setfill('0')
       << static_cast<unsigned short>(item)
       << "]\n";
  }
  os << std::endl;
}

template <typename T>
void
printVectorElements (const std::vector<T>& v) noexcept;

template <typename T>
void
printVectorElements (const std::vector<T>& v) noexcept
{
  if (v.empty())
  {
    return;
  }

  auto printItem = [] (const T& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(v.cbegin(), v.cend(), printItem);
  std::cout << "]" << std::endl;
}

template <typename T>
void
printDequeElements (const std::deque<T>& v) noexcept;

template <typename T>
void
printDequeElements (const std::deque<T>& v) noexcept
{
  if (v.empty())
  {
    return;
  }

  auto printItem = [] (const T& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(v.cbegin(), v.cend(), printItem);
  std::cout << "]" << std::endl;
}

template <typename T>
void
printValarrayElements (const std::valarray<T>& va) noexcept;

template <typename T>
void
printValarrayElements (const std::valarray<T>& va) noexcept
{
  if ( 0 == va.size() )
  {
    return;
  }

  auto printItem = [] (const T& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(std::begin(va), std::end(va), printItem);
  std::cout << "]" << std::endl;
}

template <typename C>
void
printContainerElements (const C& c) noexcept;

template <typename C>
void
printContainerElements (const C& c) noexcept
{
  if ( true == c.empty() )
  {
    return;
  }

  auto printItem = [] (const auto& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(c.cbegin(), c.cend(), printItem);
  std::cout << "]" << std::endl;
}

template <typename T, typename U>
void
printMapElements (const std::map<T,U>& m) noexcept;

template <typename T, typename U>
void
printMapElements (const std::map<T,U>& m) noexcept
{
  if (m.empty())
  {
    return;
  }

  auto printItem = [] (const auto& item)
  {
    std::cout << "[" << item.first << ": " << item.second << "] ";
  };

  std::cout << "[ ";
  std::for_each(std::begin(m), std::end(m), printItem);
  std::cout << "]" << std::endl;
}

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void
printTuple(std::basic_ostream<Ch,Tr>& os,
           const Tuple& t,
           std::index_sequence<Is...>);

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void
printTuple(std::basic_ostream<Ch,Tr>& os,
           const Tuple& t,
           std::index_sequence<Is...>)
{
  ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}
}  // namespace utilities

// overload << stream operator for std::pair's
template <typename T, typename U>
std::ostream&
operator<<(std::ostream &os, const std::pair<T,U>& p);

template <typename T, typename U>
std::ostream&
operator<<(std::ostream &os, const std::pair<T,U>& p)
{
  return os << "[" << p.first << ", " << p.second <<  "]";
}

// overload << stream operator for std::valarray's
template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::valarray<T>& va);

template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::valarray<T>& va)
{
  if ( 0 == va.size() )
  {
    return os;
  }

  auto printItem = [&os] (const T& item)
  {
    os << item << " ";
  };

  os << "[ ";
  std::for_each(std::begin(va), std::end(va), printItem);
  os << "]";
  return os;
}

// overload << stream operator for std::vector's
template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::vector<T>& v);

template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::vector<T>& v)
{
  if ( 0 == v.size() )
  {
    return os;
  }

  auto printItem = [&os] (const T& item)
  {
    os << item << " ";
  };
  
  os << "[ ";
  std::for_each(v.cbegin(), v.cend(), printItem);
  os << "]";
  return os;
}

// overload << stream operator for std::deque's
template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::deque<T>& v);

template <typename T>
std::ostream&
operator<<(std::ostream &os, const std::deque<T>& v)
{
  if ( 0 == v.size() )
  {
    return os;
  }

  auto printItem = [&os] (const T& item)
  {
    os << item << " ";
  };
  
  os << "[ ";
  std::for_each(v.cbegin(), v.cend(), printItem);
  os << "]";
  return os;
}

// overload << stream operator for std::tuple's
template<class Ch, class Tr, class... Args>
decltype(auto)
operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t);

template<class Ch, class Tr, class... Args>
decltype(auto)
operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
{
  os << "[";
  utilities::printTuple(os, t, std::index_sequence_for<Args...>{});
  return os << "]";
}

namespace utilities
{
template <typename T, typename ... Ts>
void
printArgs(std::ostream& os, const T& v, const Ts& ...vs);

template <typename T, typename ... Ts>
void
printArgs(std::ostream& os, const T& v, const Ts& ...vs)
{
  os << v;
  (void)std::initializer_list<int>{((os << ", " << vs), 0)...};
}
}  // namespace utilities
