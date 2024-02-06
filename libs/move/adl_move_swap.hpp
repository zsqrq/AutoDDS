//
// Created by wz on 24-2-5.
//

#ifndef AUTODDS_LIBS_MOVE_ADL_MOVE_SWAP_HPP_
#define AUTODDS_LIBS_MOVE_ADL_MOVE_SWAP_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include <cstddef>
#include "libs/move/detail/workaround.hpp"

#if defined(AUTODDS_GNU_STDLIB)
//For non-GCC compilers, where GNUC version is not very reliable, or old GCC versions
//use the good old stl_algobase header, which is quite lightweight
#if !defined(AUTODDS_GCC) || ((__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 3)))
#include <bits/stl_algobase.h>
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ == 3)
//In GCC 4.3 a tiny stl_move.h was created with swap and move utilities
      #include <bits/stl_move.h>
#else
//In GCC 4.4 stl_move.h was renamed to move.h
#include <bits/move.h>
#endif

#if defined(_LIBCPP_VERSION)
#include <type_traits>  //The initial import of libc++ defines std::swap and still there
#elif __cplusplus >= 201103L
   #include <utility>    //Fallback for C++ >= 2011
#else
   #include <algorithm>  //Fallback for C++98/03
#endif

#include "libs/move/utility_core.hpp"

namespace autodds_libs_move_adl_swap {

template <typename T>
AUTODDS_FORCEINLINE void swap_proxy (T& x, T& y) {
  std::swap(x, y);
}

template<class T, std::size_t N>
void swap_proxy(T (& x)[N], T (& y)[N])
{
  for (std::size_t i = 0; i < N; ++i){
    swap_proxy(x[i], y[i]);
  }
}

}

namespace autodds {
namespace libs {

template <class T>
AUTODDS_FORCEINLINE void adl_move_swap(T& x, T& y) {
  ::autodds_libs_move_adl_swap::swap_proxy(x, y);
}

template <class ForwardIt1, class ForwardIt2>
ForwardIt2 adl_move_swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
  while (first1 != last1) {
    adl_move_swap(*first1, *first2);
    ++first1;
    ++first2;
  }
  return first2;
}

template<class BidirIt1, class BidirIt2>
BidirIt2 adl_move_swap_ranges_backward(BidirIt1 first1, BidirIt1 last1, BidirIt2 last2)
{
  while (first1 != last1) {
    adl_move_swap(*(--last1), *(--last2));
  }
  return last2;
}

template<class ForwardIt1, class ForwardIt2>
void adl_move_iter_swap(ForwardIt1 a, ForwardIt2 b)
{
  adl_move_swap(*a, *b);
}

}
}
#endif
#endif //AUTODDS_LIBS_MOVE_ADL_MOVE_SWAP_HPP_