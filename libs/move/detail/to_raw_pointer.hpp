//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_TO_RAW_POINTER_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_TO_RAW_POINTER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/pointer_element.hpp"

namespace autodds {
namespace libs {
namespace movelib {

template <typename T>
AUTODDS_MOVE_FORCEINLINE T* to_raw_pointer(T* p)
{ return p;}

template <typename Pointer>
AUTODDS_MOVE_FORCEINLINE typename autodds::movelib::pointer_element<Pointer>::type*
to_raw_pointer(const Pointer& ptr)
{
  return autodds::libs::movelib::to_raw_pointer(ptr.operator->());
}

} // namespace movelib
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_DETAIL_TO_RAW_POINTER_HPP_