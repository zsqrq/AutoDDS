//
// Created by wz on 24-3-4.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_FORCE_PTR_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_FORCE_PTR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace move_detail {

template <typename T>
AUTODDS_MOVE_FORCEINLINE T force_ptr(const volatile void* p)
{
  return static_cast<T>(const_cast<void*>(p));
}

} // namespace move_detail
} // namespace libs
} // namespace move_detail

#endif //AUTODDS_LIBS_MOVE_DETAIL_FORCE_PTR_HPP_