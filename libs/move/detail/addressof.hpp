//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_ADDRESSOF_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_ADDRESSOF_HPP_

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

#if defined(AUTODDS_GCC) && AUTODDS_GCC >= 70000
#define AUTODDS_MOVE_HAS_BUILTIN_ADDRESSOF
#elif defined(__has_builtin)
#define AUTODDS_MOVE_HAS_BUILTIN_ADDRESSOF
#endif

#ifdef AUTODDS_MOVE_HAS_BUILTIN_ADDRESSOF

template <typename T>
AUTODDS_MOVE_FORCEINLINE T* addressof(T& v) AUTODDS_NOEXCEPT
{
  return __builtin_addressof(v);
}
#else

template <T>
AUTODDS_MOVE_FORCEINLINE T* addressof(T& obj)
{
  return static_cast<T*>(
      static_cast<void*>(
          const_cast<char*>(
              &reinterpret_cast<const volatile char&>(obj)
              )
          )
      );
}

#endif

}
}
}

#endif //AUTODDS_LIBS_MOVE_DETAIL_ADDRESSOF_HPP_