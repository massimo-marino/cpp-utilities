/* 
 * File:   memvar.h
 * Author: massimo
 *
 * Created on October 17, 2017, 1:50 PM
 */

#ifndef MEMVAR_H
#define MEMVAR_H

#include <tuple>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// memvar
// a variable with memory of old values
// only strings, integral or floating point numbers allowed
template <typename T>
class memvar final
{
  using memvarHistory = std::deque<T>;
  
 public:
  explicit memvar() noexcept
  {
    static_assert( (true == std::is_integral<T>::value ||
                    true == std::is_floating_point<T>::value ||
                    true == is_string<T>::value),
                  "String, integral or floating point required.");
    setValue(T{});
  }

  explicit memvar(const T& value, const int historyCapacity = historyCapacityDefault) noexcept(false)
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

  // we don't want these objects allocated on the heap
  void* operator new(std::size_t) = delete;
  void operator delete(void*) = delete;
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

  auto getHistoryCapacity() const noexcept
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

  auto isHistoryFull() const noexcept
  {
    return memo_.size() >= historyCapacity_; 
  }

  auto getMemVarHistory () const noexcept
  {
    return memo_;
  }

  auto getHistoryValue(const int index) const noexcept -> std::tuple<T, bool>
  {
    if ( (index < getHistorySize()) && (index >= 0) )
    {
      return std::make_tuple(memo_.at(index), false);
    }
    return std::make_tuple(T{}, true);
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
}  // namespace utilities

template <typename T>
std::ostream& operator<<(std::ostream& os, const utilities::memvar<T>& mv)
{
   return os << mv.getValue();
}

#endif /* MEMVAR_H */
