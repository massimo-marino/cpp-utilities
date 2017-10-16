/* 
 * File:   utilities.h
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "concurrentLogging.h"
#include "containerHelpers.h"
#include "vectorHelpers.h"
#include "objectFactory.h"
#include "printHelpers.h"

#include <thread>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
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
// a simple function's performance timer
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
    f(std::forward<Args>(args)...);
    auto end = Clock::now();

    return std::chrono::duration_cast<Time>(end - start);
  }
};
////////////////////////////////////////////////////////////////////////////////
// memvar
// a variable with memory of old values
// only integral or floating point numbers allowed
template <typename T>
class memvar final
{
 public:
  memvar() noexcept
  {
    setValue(T{});
  }

  memvar(const T& value, const int histCapacity = histCapacityDefault) noexcept(false)
  :
  histCapacity_ (histCapacity)
  {
    if ( histCapacity_ <= 0 )
    {
      throw std::invalid_argument("ERROR: The history capacity must not be zero or negative");
    }

    setValue(value);
  }

  ~memvar() = default;
  memvar(const memvar& rhs) = delete;
  memvar& operator=(const memvar& rhs) = delete;
  memvar(const memvar&& rhs) = delete;
  memvar& operator=(const memvar&& rhs) = delete;

  memvar& operator=(const T& rhs) noexcept
  {
    setValue(rhs);
    return *this;
  }

  T operator()() const noexcept
  {
    return getValue();
  }

  // ++mv
  T operator++() const noexcept
  {
    setValue(getValue() + 1);
    return getValue();
  }

  // mv++
  T operator++(int dummy) const noexcept
  {
    setValue(getValue() + 1);
    return getValue();
  }  

    // --mv
  T operator--() const noexcept
  {
    setValue(getValue() - 1);
    return getValue();
  }

  // mv--
  T operator--(int dummy) const noexcept
  {
    setValue(getValue() - 1);
    return getValue();
  }

  void printHistData() const noexcept
  {
    utilities::printVectorElements(mem_);
  }

  int getHistCapacity() const noexcept
  {
    return histCapacity_;
  }

  auto getHistSize() const noexcept
  {
    return mem_.size();
  }

  void clearHistory() const noexcept
  {
    mem_.erase(begin(mem_) + 1, end(mem_));
  }

  bool isHistoryFull() const noexcept
  {
    return mem_.size() >= histCapacity_; 
  }

 private:
  static const int histCapacityDefault {10};
  const int histCapacity_ {histCapacityDefault};
  mutable std::vector<T> mem_ {};

  void setValue(const T& value) const noexcept
  {
    mem_.emplace(begin(mem_), value);
    if ( mem_.size() > getHistCapacity() )
    {
      mem_.pop_back();
    }
  }

  T getValue() const noexcept
  {
    return mem_.at(0);
  }

  void setHistCapacity(const int newHistCapacity = histCapacityDefault) const noexcept
  {
    histCapacity_ = newHistCapacity;
  }
};  // memvar
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities

template <typename T>
std::ostream& operator<<(std::ostream& os, const utilities::memvar<T>& mv)
{
   return os << mv.getValue();
}

////////////////////////////////////////////////////////////////////////////////
#endif /* UTILITIES_H */
