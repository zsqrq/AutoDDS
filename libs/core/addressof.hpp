//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_CORE_ADDRESSOF_HPP_
#define AUTODDS_LIBS_CORE_ADDRESSOF_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if defined(AUTODDS_MSVC_FULL_VER) && AUTODDS_MSVC_FULL_VER >= 190024215
#define AUTODDS_CORE_HAS_BUILTIN_ADDRESSOF
#elif defined(AUTODDS_GCC) && AUTODDS_GCC >= 70000
#define AUTODDS_CORE_HAS_BUILTIN_ADDRESSOF
#elif defined(__has_builtin)
#if __has_builtin(__builtin_addressof)
#define AUTODDS_CORE_HAS_BUILTIN_ADDRESSOF
#endif
#endif

#if defined(AUTODDS_CORE_HAS_BUILTIN_ADDRESSOF)

namespace autodds {
namespace libs {

template<typename T>
AUTODDS_CONSTEXPR inline T* addressof(T& p) AUTODDS_NOEXCEPT
{
  return __builtin_addressof(p);
}

template<class T>
const T* addressof(const T&&) = delete;

} // namespace autodds
} // namespace libs

#endif

#endif //AUTODDS_LIBS_CORE_ADDRESSOF_HPP_