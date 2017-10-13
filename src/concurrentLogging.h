/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   concurrentLogging.h
 * Author: massimo
 *
 * Created on October 13, 2017, 8:27 AM
 */

#ifndef CONCURRENTLOGGING_H
#define CONCURRENTLOGGING_H

#include <iostream>
#include <sstream>
#include <mutex>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
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

}  // namespace utilities

#endif /* CONCURRENTLOGGING_H */
