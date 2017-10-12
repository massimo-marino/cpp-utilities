/* 
 * File:   utilities.h
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>
#include <experimental/string_view>
#include <experimental/algorithm>
#include <chrono>
#include <type_traits>
#include <functional>
//#include <algorithm>
#include <valarray>
#include <tuple>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
////////////////////////////////////////////////////////////////////////////////
// helpers for concurrent logging
struct pclog : public std::stringstream
{
  static inline std::mutex clog_mutex;

  pclog() = default;
  pclog(const pclog& rhs) = delete;
  ~pclog();
};

struct pcout : public std::stringstream
{
  static inline std::mutex cout_mutex;

  pcout() = default;
  pcout(const pcout& rhs) = delete;
  ~pcout();
};

struct pcerr : public std::stringstream
{
  static inline std::mutex cerr_mutex;

  pcerr() = default;
  pcerr(const pcerr& rhs) = delete;
  ~pcerr();
};
////////////////////////////////////////////////////////////////////////////////

void yieldCPUAndSleep (const int64_t& nanoseconds = 0) noexcept;
void removeMultipleOccurrences(std::string& s) noexcept;

////////////////////////////////////////////////////////////////////////////////
// check if all the parameters are within a certain range
template <typename T, typename ... Ts>
bool within(const T& min, const T& max, const Ts& ...ts) noexcept
{
  return ((min <= ts && ts <= max) && ...);
}

////////////////////////////////////////////////////////////////////////////////
// vector related helpers

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

// pushing multiple items into a vector
template <typename T, typename ... Ts>
void insertAll(std::vector<T>& vec, Ts ... ts)  noexcept
{
  (vec.push_back(ts), ...);
}

// deleting items from an unsorted std::vector in O(1) time
// Pre-Condition: the vector is NOT sorted
template <typename T>
void quickRemoveAt(std::vector<T>& v, const std::size_t idx) noexcept
{
  if ( idx < v.size() )
  {
    v.at(idx) = std::move(v.back());
    v.pop_back();
  }
}

// deleting items from an unsorted std::vector in O(1) time
// Pre-Condition: the vector is NOT sorted
template <typename T>
void quickRemoveAt(std::vector<T>& v, typename std::vector<T>::iterator it) noexcept
{
  if ( it != std::end(v) )
  {
    *it = std::move(v.back());
    v.pop_back();
  }
}

// deleting items from an unsorted std::vector in O(1) time
// Pre-Condition: the vector is NOT sorted
template <typename T>
void quickRemove(std::vector<T>& v, const T& item) noexcept
{
  auto it = std::find(std::begin(v), std::end(v), item);
  if ( it != std::end(v) )
  {
    *it = std::move(v.back());
    v.pop_back();
  }
}

// using the erase-remove idiom to remove all occurrences of an item from a std::vector
template <typename T>
void removeAllItems (std::vector<T>& v, const T& item) noexcept
{
  v.erase(std::remove(std::begin(v), std::end(v), item), end(v));
  v.shrink_to_fit();
}

////////////////////////////////////////////////////////////////////////////////
// generic helpers for containers

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

// insert an item of type T in a sorted container C
// Pre-Condition: the container MUST be sorted
template <typename C, typename T>
void insertSorted(C &c, const T &item) noexcept
{
  const auto insert_pos (std::lower_bound(std::begin(c), std::end(c), item));
  c.insert(insert_pos, item);
}

template <typename K, typename V>
auto insertInMap(std::map<K,V>& m, const K& key, const V& value) noexcept
{
  // C++17, not yet available
  //return m.try_emplace(key, value);

  // C++11
  return m.emplace(key, value);
}

template <typename T>
std::valarray<T> innerProduct(const std::valarray<T>& va1,
                              const std::valarray<T>& va2) noexcept
{
  return (va1 * va2);
}

template <typename T>
T dotProduct(const std::valarray<T>& va1,
             const std::valarray<T>& va2) noexcept
{
  return innerProduct(va1, va2).sum();
}

template<typename Array, std::size_t... I>
decltype(auto) arrayToTuple_impl(const Array& a, std::index_sequence<I...>)
{
  return std::make_tuple(a[I]...);
}

// Convert array into a tuple
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
decltype(auto) arrayToTuple(const std::array<T, N>& a)
{
  return arrayToTuple_impl(a, Indices{});
}

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void printTuple(std::basic_ostream<Ch,Tr>& os,
                const Tuple& t,
                std::index_sequence<Is...>)
{
  ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}

////////////////////////////////////////////////////////////////////////////////

template <typename Time = std::chrono::nanoseconds,
          typename Clock = std::chrono::high_resolution_clock>
struct perftimer
{
  template <typename F, typename... Args>
  static Time duration(F&& f, Args... args)
  {
    auto start = Clock::now();
    // C++17: not yet available
    //std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    f( std::forward<Args>(args)...);
    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }
};
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities
////////////////////////////////////////////////////////////////////////////////

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
////////////////////////////////////////////////////////////////////////////////
#endif /* UTILITIES_H */
