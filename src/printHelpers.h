/* 
 * File:   printHelpers.h
 * Author: massimo
 *
 * Created on October 13, 2017, 8:36 AM
 */

#ifndef PRINTHELPERS_H
#define PRINTHELPERS_H

#include <map>
#include <valarray>
#include <deque>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
template <typename T>
void printVectorElements (const std::vector<T>& v) noexcept
{
  if ( true == v.empty() )
  {
    return;
  }

  auto printItem = [] (const T& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(std::begin(v), std::end(v), printItem);
  std::cout << "]" << std::endl;
}

template <typename T>
void printDequeElements (const std::deque<T>& v) noexcept
{
  if ( true == v.empty() )
  {
    return;
  }

  auto printItem = [] (const T& item)
  {
    std::cout << item << " ";
  };

  std::cout << "[ ";
  std::for_each(std::begin(v), std::end(v), printItem);
  std::cout << "]" << std::endl;
}

template <typename T>
void printValarrayElements (const std::valarray<T>& va) noexcept
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
void printContainerElements (const C& c) noexcept
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
  std::for_each(std::begin(c), std::end(c), printItem);
  std::cout << "]" << std::endl;
}

template <typename T, typename U>
void printMapElements (const std::map<T,U>& m) noexcept
{
  if ( true == m.empty() )
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
void printTuple(std::basic_ostream<Ch,Tr>& os,
                const Tuple& t,
                std::index_sequence<Is...>)
{
  ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}
}  // namespace utilities

// overload << stream operator for std::pair's
template <typename T, typename U>
std::ostream& operator<<(std::ostream &os, const std::pair<T,U>& p)
{
  return os << "[" << p.first << ", " << p.second <<  "]";
}

// overload << stream operator for std::valarray's
template <typename T>
std::ostream& operator<<(std::ostream &os, const std::valarray<T>& va)
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
std::ostream& operator<<(std::ostream &os, const std::vector<T>& v)
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
  std::for_each(std::begin(v), std::end(v), printItem);
  os << "]";
  return os;
}

// overload << stream operator for std::vector's
template <typename T>
std::ostream& operator<<(std::ostream &os, const std::deque<T>& v)
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
  std::for_each(std::begin(v), std::end(v), printItem);
  os << "]";
  return os;
}

template<class Ch, class Tr, class... Args>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,
                          const std::tuple<Args...>& t)
{
  os << "[";
  utilities::printTuple(os, t, std::index_sequence_for<Args...>{});
  return os << "]";
}

namespace utilities
{
template <typename T, typename ... Ts>
void printArgs(std::ostream& os, const T& v, const Ts& ...vs)
{
  os << v;
  (void)std::initializer_list<int>{((os << ", " << vs), 0)...};
}
}  // namespace utilities

#endif /* PRINTHELPERS_H */

