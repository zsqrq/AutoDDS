//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_TRAITS_HPP_
#define AUTODDS_LIBS_MOVE_TRAITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/core.hpp"
#include "libs/move/detail/meta_utils.hpp"
#include "libs/move/detail/type_traits.hpp"

namespace autodds {
namespace libs {

//! If this trait yields to true
//! (<i>has_trivial_destructor_after_move &lt;T&gt;::value == true</i>)
//! means that if T is used as argument of a move construction/assignment,
//! there is no need to call T's destructor.
//! This optimization tipically is used to improve containers' performance.
//!
//! By default this trait is true if the type has trivial destructor,
//! every class should specialize this trait if it wants to improve performance
//! when inserted in containers.
template <typename T>
struct has_trivial_destructor_after_move : move_detail::is_trivially_destructible<T>
{};

//! By default this traits returns
//! <pre> is_nothrow_move_constructible<T>::value && is_nothrow_move_assignable<T>::value </pre>.
//! Classes with non-throwing move constructor
//! and assignment can specialize this trait to obtain some performance improvements.
template <typename T>
struct has_nothrow_move
{
  static const bool value = move_detail::is_nothrow_move_constructible<T>::value &&
      move_detail::is_nothrow_copy_assignable<T>::value;
};

namespace move_detail {

template <typename T>
struct is_nothrow_move_constructible_or_uncopyable
{
  static const bool value = is_nothrow_copy_constructible<T>::value ||
                            has_nothrow_move<T>::value||
                            !is_copy_constructible<T>::value;
};

} // namespace move_detail

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_TRAITS_HPP_