/* 
 * File:   randomNumberGenerators.h
 * Author: massimo
 *
 * Created on November 21, 2017, 6:57 PM
 */

#ifndef RANDOMNUMBERGENERATORS_H
#define RANDOMNUMBERGENERATORS_H

#include <random>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
template <typename T = double>
static
T
getRandomFP(const T min, const T max)
{
  static_assert((true == std::is_floating_point<T>::value),
                 "a floating point type required.");

  // used to obtain a seed for the random number engine
  std::random_device rd;

  // standard mersenne_twister_engine seeded with rd()
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}

template <typename T = int>
static
T
getRandomINT(const T min, const T max)
{
  static_assert((true == std::is_integral<T>::value),
                 "an integral type required.");

  // will be used to obtain a seed for the random number engine
  std::random_device rd;

  //Standard mersenne_twister_engine seeded with rd()
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}
}  // namespace utilities

#endif /* RANDOMNUMBERGENERATORS_H */
