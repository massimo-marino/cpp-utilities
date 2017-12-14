/* 
 * File:   vectorHelpers.h
 * Author: massimo
 *
 * Created on October 13, 2017, 8:58 AM
 */
#pragma once

#include <vector>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// pushing multiple items into a vector
template <typename T, typename ... Ts>
void
insertAll(std::vector<T>& vec, Ts ... ts)  noexcept
{
  (vec.push_back(ts), ...);
}

// deleting items from an unsorted std::vector in O(1) time
// Pre-Condition: the vector is NOT sorted
template <typename T>
void
quickRemoveAt(std::vector<T>& v, const std::size_t idx) noexcept
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
void
quickRemoveAt(std::vector<T>& v, typename std::vector<T>::iterator it) noexcept
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
void
quickRemove(std::vector<T>& v, const T& item) noexcept
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
void
removeAllItems (std::vector<T>& v, const T& item) noexcept
{
  v.erase(std::remove(std::begin(v), std::end(v), item), end(v));
  v.shrink_to_fit();
}

}  // namespace utilities
