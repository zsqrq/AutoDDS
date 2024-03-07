//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_MATH_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_MATH_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
#include <climits>
#include <cstring>
#include "libs/intrusive/detail/mpl.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

//Compile-time error in case of missing specialization
template<class Uint>
struct builtin_clz_dispatch;

#if defined(AUTODDS_HAS_LONG_LONG)
template<>
   struct builtin_clz_dispatch< ::autodds::ulong_long_type >
   {
      static ::autodds::ulong_long_type call(::autodds::ulong_long_type n)
      {  return (::autodds::ulong_long_type)__builtin_clzll(n); }
   };
#endif

template<>
struct builtin_clz_dispatch<unsigned long>
{
  static unsigned long call(unsigned long n)
  {  return (unsigned long)__builtin_clzl(n); }
};

template<>
struct builtin_clz_dispatch<unsigned int>
{
  static unsigned int call(unsigned int n)
  {  return (unsigned int)__builtin_clz(n); }
};

inline std::size_t floor_log2(std::size_t n)
{
  return sizeof(std::size_t)*CHAR_BIT - std::size_t(1) - builtin_clz_dispatch<std::size_t>::call(n);
}

//Thanks to Laurent de Soras in
//http://www.flipcode.com/archives/Fast_log_Function.shtml
inline float fast_log2 (float val)
{
  unsigned x;
  std::memcpy(&x, &val, sizeof(float));
  const int log_2 = int((x >> 23) & 255) - 128;
  x &= ~(unsigned(255u) << 23u);
  x += unsigned(127) << 23u;
  std::memcpy(&val, &x, sizeof(float));
  //1+log2(m), m ranging from 1 to 2
  //3rd degree polynomial keeping first derivate continuity.
  //For less precision the line can be commented out
  val = ((-1.f/3.f) * val + 2.f) * val - (2.f/3.f);
  return val + static_cast<float>(log_2);
}

inline bool is_pow2(std::size_t x)
{  return (x & (x-1)) == 0;  }

template<std::size_t N>
struct static_is_pow2
{
  static const bool value = (N & (N-1)) == 0;
};

inline std::size_t ceil_log2 (std::size_t x)
{
  return static_cast<std::size_t>(!(is_pow2)(x)) + floor_log2(x);
}

inline std::size_t ceil_pow2 (std::size_t x)
{
  return std::size_t(1u) << (ceil_log2)(x);
}

template<typename SizeType, std::size_t N>
struct numbits_eq
{
  static const bool value = sizeof(SizeType)*CHAR_BIT == N;
};

template<typename SizeType, typename Enabler = void >
struct sqrt2_pow_max;

using ::autodds::intrusive::detail::voider;
using ::autodds::intrusive::detail::enable_if;

template <typename SizeType>
struct sqrt2_pow_max<SizeType, typename ::autodds::intrusive::detail::voider<
    typename ::autodds::intrusive::detail::enable_if< numbits_eq<SizeType, 32> >::type>::type>
{
  static const SizeType value = 0xb504f334;
  static const std::size_t pow   = 31;
};

template <class SizeType>
struct sqrt2_pow_max<SizeType, typename voider<typename enable_if< numbits_eq<SizeType, 64> >::type>::type>
{
  static const SizeType value = 0xb504f333f9de6484ull;
  static const std::size_t pow   = 63;
};

// Returns floor(pow(sqrt(2), x * 2 + 1)).
// Defined for X from 0 up to the number of bits in size_t minus 1.
inline std::size_t sqrt2_pow_2xplus1 (std::size_t x)
{
  const std::size_t value = (std::size_t)sqrt2_pow_max<std::size_t>::value;
  const std::size_t pow   = (std::size_t)sqrt2_pow_max<std::size_t>::pow;
  return (value >> (pow - x)) + 1;
}

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_MATH_HPP_