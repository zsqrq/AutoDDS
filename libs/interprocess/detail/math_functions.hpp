//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_MATH_FUNCTIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_MATH_FUNCTIONS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/common/static_assert.hpp"
#include <climits>

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

// gcd is an algorithm that calculates the greatest common divisor of two
//  integers, using Euclid's algorithm.
template<typename Integer>
inline Integer gcd(Integer A, Integer B)
{
  do {
    const Integer tmp(B);
    B = A % B;
    A = tmp;
  } while (B != 0);
  return A;
}

// lcm is an algorithm that calculates the least common multiple of two
template<typename Integer>
inline Integer lcm(const Integer & A, const Integer & B)
{
  Integer ret = A;
  ret /= gcd(A, B);
  ret *= B;
  return ret;
}

template <typename Integer>
inline Integer log2_ceil(const Integer & A)
{
  Integer i = 0;
  Integer power_of_2 = 1;

  while(power_of_2 < A){
    power_of_2 <<= 1;
    ++i;
  }
  return i;
}

template <typename Integer>
inline Integer upper_power_of_2(const Integer & A)
{
  Integer power_of_2 = 1;

  while(power_of_2 < A){
    power_of_2 <<= 1;
  }
  return power_of_2;
}

//This function uses binary search to discover the
//highest set bit of the integer
inline std::size_t floor_log2 (std::size_t x)
{
  const std::size_t Bits = sizeof(std::size_t)*CHAR_BIT;
  const bool Size_t_Bits_Power_2= !(Bits & (Bits-1));
  AUTODDS_STATIC_ASSERT(((Size_t_Bits_Power_2)== true));

  std::size_t n = x;
  std::size_t log2 = 0;

  for(std::size_t shift = Bits >> 1; shift; shift >>= 1){
    std::size_t tmp = n >> shift;
    if (tmp)
      log2 += shift, n = tmp;
  }

  return log2;
}

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_MATH_FUNCTIONS_HPP_