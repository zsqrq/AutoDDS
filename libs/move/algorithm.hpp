//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_ALGORITHM_HPP_
#define AUTODDS_LIBS_MOVE_ALGORITHM_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include "libs/move/utility_core.hpp"
#include "libs/move/iterator.hpp"
#include "libs/move/algo/move.hpp"

#include <algorithm>
#include <memory>

namespace autodds {
namespace libs {
namespace move_detail {

template <typename I, typename F>
inline F uninitialized_move_move_iterator(I f, I l, F r)
{
  return autodds::libs::uninitialized_move(f, l, r);
}

} // namespace move_detail

template <typename I, typename F>
inline F uninitialized_copy_or_move(I f, I l, F r,
                                    typename move_detail::enable_if<move_detail::is_move_iterator<I>>::type* = 0)
{
  return move_detail::uninitialized_move_move_iterator(f, l, r);
}

namespace move_detail {

template <typename I, typename F>
inline F move_move_iterator(I f, I l, F r)
{
  return autodds::libs::move(f, l, r);
}

} // namespace move_detail

template <typename I,   // I models InputIterator
        typename F>   // F models ForwardIterator
inline F copy_or_move(I f, I l, F r,
                      typename move_detail::enable_if< move_detail::is_move_iterator<I> >::type* = 0)
{
  return move_detail::move_move_iterator(f, l, r);
}

template<typename I,   // I models InputIterator
        typename F>   // F models ForwardIterator
inline F uninitialized_copy_or_move(I f, I l, F r
    /// @cond
    ,typename move_detail::disable_if< move_detail::is_move_iterator<I> >::type* = 0
    /// @endcond
)
{
  return std::uninitialized_copy(f, l, r);
}

template<typename I,   // I models InputIterator
        typename F>   // F models ForwardIterator
inline F copy_or_move(I f, I l, F r
    /// @cond
    ,typename move_detail::disable_if< move_detail::is_move_iterator<I> >::type* = 0
    /// @endcond
)
{
  return std::copy(f, l, r);
}

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_ALGORITHM_HPP_