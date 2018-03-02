/* 
 * File:   randomNumberGenerators.h
 * Author: massimo
 *
 * Created on November 21, 2017, 6:57 PM
 */
#pragma once

#include <random>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// Seed a random number engine with greater unpredictability.
//
// A pseudo-random number engine is initialized with a seed, which
// determines the initial state of the engine. When seeding with
// a single 32 bit value (as is typically provided by
// [std::random_device](cpp/numeric/random/random_device)), the
// engine can be in one of only 2<sup>32</sup> states. By providing an
// an engine with a greater amount of random seed data, we increase
// the number of possible states and therefore the unpredictability
// of the engine.
//
// With std::random_device rd;, we create a `std::random_device` as a source of
// non-deterministic random numbers (if such a source is available).
// With std::seed_seq seed_seq {rd(), rd(), rd(), rd(), rd(), rd()};
// we initialize a [`std::seed_seq`](cpp/numeric/random/seed_seq)
// with several initial seeds produced by the `std::random_device`.
// With static std::mt19937 gen {seed_seq}; we seed a random number engine with
// this `std::seed_seq`, which provides the engine with a sequence of evenly
// distributed seeds with low bias (although [not bias-free] (http://www.pcg-random.org/posts/cpp-seeding-surprises.html)).
// Providing more initial seeds will increase the unpredictability of
// the engine by increasing the number of possible seed sequences that
// might be generated.
//
// While a `std::seed_seq` can be used to initialize multiple random number
// engines, bear in mind that a particular `std::seed_seq` will produce the same
// sequence of seeds each time it is used.
//
// **Note**: An unpredictable random number engine is not necessarily
// cryptographically secure.
template <typename T = double>
const
T
getRandomFP(T min, T max) noexcept
{
  static_assert(std::is_floating_point<T>::value != 0, "a floating point type required.");

  // used to obtain a seed for the random number engine
  std::random_device rd;

  // standard mersenne_twister_engine seeded with rd()
  std::seed_seq seed_seq {rd(), rd(), rd(), rd(), rd(), rd()};
  static std::mt19937 gen {seed_seq};

  static std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}

template <typename T = int>
const
T
getRandomINT(const T min, const T max) noexcept
{
  static_assert(std::is_integral<T>::value != 0, "an integral type required.");

  // will be used to obtain a seed for the random number engine
  std::random_device rd;

  //Standard mersenne_twister_engine seeded with rd()
  std::seed_seq seed_seq {rd(), rd(), rd(), rd(), rd(), rd()};
  static std::mt19937 gen {seed_seq};

  static std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}
}  // namespace utilities
