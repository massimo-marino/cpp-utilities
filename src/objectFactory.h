/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   objectFactory.h
 * Author: massimo
 *
 * Created on October 12, 2017, 3:47 PM
 */

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <memory>
////////////////////////////////////////////////////////////////////////////////
namespace utilities::object_factory
{
// create an object of type T and return a std::unique_ptr to it
template <typename T, typename... Args>
auto createUniquePtr(Args&&... args) -> std::unique_ptr<T>
{
  // since C++14
  return std::make_unique<T>(args...);
}

template <typename T>
using objectFactoryFun = std::function<std::unique_ptr<T>(void)>;

template <typename T, typename... Args>
auto createObjectFactoryFun(Args&&... args) noexcept -> objectFactoryFun<T>
{
  // return a function object for creating T's objects with the given arguments
  // to be passed to its constructor
  return [args...]()
         {
           return createUniquePtr<T>(std::forward<Args>(args)...);
         };
}

template <typename T>
std::shared_ptr<T> uniquePtr2sharedPtr(std::unique_ptr<T>& uptr) noexcept
{
  if (uptr)
  {
    return std::shared_ptr<T>(uptr.release());
  }
  return nullptr;
}
////////////////////////////////////////////////////////////////////////////////
}  // namespace utilities::object_factory
#endif /* OBJECTFACTORY_H */

