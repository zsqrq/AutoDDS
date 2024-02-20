//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_MPL_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_MPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <typename T, T val>
struct integral_constant
{
  static const T value = val;
  typedef integral_constant<T, val> type;
};

template <bool C_>
struct bool_ : integral_constant<bool, C_>
{
  static const bool value = C_;
};

typedef bool_<true>  true_;
typedef bool_<false> false_;
typedef true_  true_type;
typedef false_ false_type;

typedef char yes_type;
struct no_type
{ char padding[8]; };

template <bool B, typename T = void>
struct enable_if_c
{ typedef T type; };

template <typename T>
struct enable_if_c<false,T> {};

template <typename Cond, typename T =void>
struct enable_if : public enable_if_c<Cond::value, T> {};

template <typename Cond, typename T = void>
struct disable_if : public enable_if_c<!Cond::value, T> {};

template <
    bool C,
    typename T1,
    typename T2>
struct if_c
{
  typedef T1 type;
};

template <typename T1, typename T2>
struct if_c<false, T1, T2>
{ typedef T2 type; };

template <typename T1, typename T2, typename T3>
struct if_
{ typedef typename if_c<0 != T1::value, T2, T3>::type type; };

template <std::size_t S>
struct ls_zeros
{
  static const std::size_t value = (S & std::size_t(1)) ? 0 : (1u + ls_zeros<(S >> 1u)>::value);
};

template <>
struct ls_zeros<0>
{
  static const std::size_t value = 0;
};

template <>
struct ls_zeros<1>
{
  static const std::size_t value = 1;
};


} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_MPL_HPP_