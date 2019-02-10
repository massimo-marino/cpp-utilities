//
// bracketedLog.h
//
// Created by massimo on 2/8/19.
//

#pragma once

#include <iostream>
////////////////////////////////////////////////////////////////////////////////
class bracketedLog final
{
public:

  explicit
  bracketedLog(const std::string& funcName, std::ostream& os = std::cout) noexcept
  : funcName_(funcName),
    os_(os)
  {
    os_ << "[ "
        << funcName_
        << " started ]"
        << std::endl;
  }
  explicit
  bracketedLog(const std::string& funcName,
               const std::string& fileName,
               const unsigned int numLine,
               std::ostream& os = std::cout) noexcept
  : funcName_(funcName),
    os_ (os)
  {
    os_ << "[ "
        << fileName << ":"
        << funcName_ << "@"
        << numLine
        << " started ]"
        << std::endl;
  }

  ~bracketedLog()
  {
    os_ << "[ "
        << funcName_
        << " ended ]"
        << std::endl;
  }

  bracketedLog() = delete;
  bracketedLog(const bracketedLog& rhs) = delete;
  bracketedLog& operator=(const bracketedLog& rhs) = delete;
  bracketedLog(bracketedLog&& rhs) = delete;
  bracketedLog& operator=(bracketedLog&& rhs) = delete;

  // we don't want these objects allocated on the heap
  void* operator new(std::size_t) = delete;
  void* operator new[](std::size_t) = delete;

  void operator delete(void*) = delete;
  void operator delete[](void*) = delete;

private:
  std::string funcName_ {};
  std::ostream& os_ {std::cout};
};  // class bracketedLog

#define BL   bracketedLog bl(__func__)
#define BLFL bracketedLog bl(__func__, __FILE__, __LINE__)
