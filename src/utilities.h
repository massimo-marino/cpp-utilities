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

#include <type_traits>
#include <thread>

#ifndef NO_DEMANGLE
#include <cxxabi.h>

namespace utilities
{
template <typename T>
std::string
type (void)
{
  std::string result;
  int         status;
  char*       demangled = abi::__cxa_demangle(typeid(T).name(),
                                              nullptr, nullptr, &status);

  result = demangled;
  free(demangled);

  return result;
}
#else
// no demangling
template <typename T>
std::string
type (void)
{
  return typeid(T).name();
}
#endif

template <typename T>
struct is_string
{
  static inline const bool value = std::is_same<T, std::string>::value ||
                                   std::is_same<T, std::wstring>::value ||
                                   std::is_same<T, std::u16string>::value ||
                                   std::is_same<T, std::u32string>::value;
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
// only strings, integral or floating point numbers allowed
template <typename T>
class memvar final
{
  using memvarHistory = std::deque<T>;
  
 public:
  memvar() noexcept
  {
    static_assert( (true == std::is_integral<T>::value ||
                    true == std::is_floating_point<T>::value ||
                    true == is_string<T>::value),
                  "String, integral or floating point required.");
    setValue(T{});
  }

  memvar(const T& value, const int historyCapacity = historyCapacityDefault) noexcept(false)
  :
  historyCapacity_ (historyCapacity)
  {
    static_assert( (true == std::is_integral<T>::value ||
                    true == std::is_floating_point<T>::value ||
                    true == is_string<T>::value),
                  "String, integral or floating point required.");
    if ( historyCapacity_ <= 0 )
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

  T incr1() const noexcept
  {
    T newValue = getValue() + 1;

    setValue(newValue);
    return newValue;    
  }

  // ++mv
  T operator++() const noexcept
  {
    return incr1();
  }

  // mv++
  T operator++(int dummy) const noexcept
  {
    return incr1();
  }  

  T decr1() const noexcept
  {
    T newValue = getValue() - 1;

    setValue(newValue);
    return newValue;    
  }

  // --mv
  T operator--() const noexcept
  {
    return decr1();
  }

  // mv--
  T operator--(int dummy) const noexcept
  {
    return decr1();
  }

  void printHistoryData() const noexcept
  {
    utilities::printDequeElements(memo_);
  }

  int getHistoryCapacity() const noexcept
  {
    return historyCapacity_;
  }

  auto getHistorySize() const noexcept
  {
    return memo_.size();
  }

  void clearHistory() const noexcept
  {
    memo_.erase(begin(memo_) + 1, end(memo_));
    memo_.shrink_to_fit();
  }

  bool isHistoryFull() const noexcept
  {
    return memo_.size() >= historyCapacity_; 
  }

  memvarHistory getMemVarHistory () const noexcept
  {
    return memo_;
  }

 private:
  static const int historyCapacityDefault {10};
  const int historyCapacity_ {historyCapacityDefault};
  mutable memvarHistory memo_ {};

  void setValue(const T& value) const noexcept
  {
    memo_.emplace(begin(memo_), value);
    if ( memo_.size() > getHistoryCapacity() )
    {
      memo_.pop_back();
    }
  }

  T getValue() const noexcept
  {
    return memo_.at(0);
  }

  void setHistoryCapacity(const int newHistoryCapacity = historyCapacityDefault) const noexcept
  {
    historyCapacity_ = newHistoryCapacity;
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
