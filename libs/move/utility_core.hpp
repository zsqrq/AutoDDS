//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_MOVE_UTILITY_CORE_HPP_
#define AUTODDS_LIBS_MOVE_UTILITY_CORE_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config//config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/meta_utils.hpp"
#include "libs/move/core.hpp"
#include <utility>

namespace autodds {
namespace libs {
using std::move;
using std::forward;

template<class T>
AUTODDS_FORCEINLINE T&& move_if_not_lvalue_reference(typename ::autodds::libs::move_detail::remove_reference<T>::type &t) AUTODDS_NOEXCEPT
{ return static_cast<T&&>(t); }

template<class T>
AUTODDS_FORCEINLINE T&& move_if_not_lvalue_reference(typename ::autodds::libs::move_detail::remove_reference<T>::type&& t) AUTODDS_NOEXCEPT
{
  AUTODDS_MOVE_STATIC_ASSERT(!autodds::libs::move_detail::is_lvalue_reference<T>::value);
  return static_cast<T&&>(t);
}

namespace move_detail {
template <typename T>
typename autodds::libs::move_detail::add_rvalue_reference<T>::type declval();
}

}
}


#endif //AUTODDS_LIBS_MOVE_UTILITY_CORE_HPP_