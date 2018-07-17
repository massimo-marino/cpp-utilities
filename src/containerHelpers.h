/* 
 * File:   containerHelpers.h
 * Author: massimo
 *
 * Created on October 13, 2017, 9:29 AM
 */
#pragma once

#include <tuple>
#include <map>
#include <array>
#include <valarray>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// insert an item of type T in a sorted container C
// Pre-Condition: the container MUST be sorted
template <typename C, typename T>
void
insertSorted(C& c, const T& item) noexcept;

template <typename C, typename T>
void
insertSorted(C& c, const T& item) noexcept
{
  const auto insert_pos (std::lower_bound(std::cbegin(c), std::cend(c), item));
  c.insert(insert_pos, item);
}

template <typename K, typename V>
auto
insertInMap(std::map<K,V>& m, const K& key, const V& value) noexcept;

template <typename K, typename V>
auto
insertInMap(std::map<K,V>& m, const K& key, const V& value) noexcept
{
  // C++17, not yet available
  //return m.try_emplace(key, value);

  // C++11
  return m.emplace(key, value);
}

template <typename T>
std::valarray<T>
innerProduct(const std::valarray<T>& va1,
             const std::valarray<T>& va2) noexcept;

template <typename T>
std::valarray<T>
innerProduct(const std::valarray<T>& va1,
             const std::valarray<T>& va2) noexcept
{
  return (va1 * va2);
}

template <typename T>
T
dotProduct(const std::valarray<T>& va1,
           const std::valarray<T>& va2) noexcept;

template <typename T>
T
dotProduct(const std::valarray<T>& va1,
           const std::valarray<T>& va2) noexcept
{
  return innerProduct(va1, va2).sum();
}

template<typename Array, std::size_t... I>
decltype(auto)
arrayToTuple_impl(const Array& a, std::index_sequence<I...>);

template<typename Array, std::size_t... I>
decltype(auto)
arrayToTuple_impl(const Array& a, std::index_sequence<I...>)
{
  return std::make_tuple(a[I]...);
}

// Convert array into a tuple
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
decltype(auto)
arrayToTuple(const std::array<T, N>& a);

template<typename T, std::size_t N, typename Indices>
decltype(auto)
arrayToTuple(const std::array<T, N>& a)
{
  return arrayToTuple_impl(a, Indices{});
}

}  // namespace utilities
