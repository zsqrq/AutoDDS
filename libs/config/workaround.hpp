//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_WORKAROUND_HPP_
#define AUTODDS_LIBS_CONFIG_WORKAROUND_HPP_
#ifndef AUTODDS_STRICT_CONFIG
#include "libs/config/config.hpp"

#ifndef __GNUC__
#define __GNUC___WORKAROUND_GUARD 1
#else
#define __GNUC___WORKAROUND_GUARD 0
#endif
#ifndef __GNUC_MINOR__
#define __GNUC_MINOR___WORKAROUND_GUARD 1
#else
#define __GNUC_MINOR___WORKAROUND_GUARD 0
#endif
#ifndef __GNUC_PATCHLEVEL__
#define __GNUC_PATCHLEVEL___WORKAROUND_GUARD 1
#else
#define __GNUC_PATCHLEVEL___WORKAROUND_GUARD 0
#endif
#ifndef AUTODDS_GCC
#define AUTODDS_GCC_WORKAROUND_GUARD 1
#define AUTODDS_GCC_VERSION_WORKAROUND_GUARD 1
#else
#define AUTODDS_GCC_WORKAROUND_GUARD 0
#define AUTODDS_GCC_VERSION_WORKAROUND_GUARD 0
#endif

#ifndef _COMPILER_VERSION
#define _COMPILER_VERSION_WORKAROUND_GUARD 1
#else
#define _COMPILER_VERSION_WORKAROUND_GUARD 0
#endif
#ifndef __clang_major__
#define __clang_major___WORKAROUND_GUARD 1
#else
#define __clang_major___WORKAROUND_GUARD 0
#endif

#ifndef __GLIBCPP__
#define __GLIBCPP___WORKAROUND_GUARD 1
#else
#define __GLIBCPP___WORKAROUND_GUARD 0
#endif
#ifndef _GLIBCXX_USE_C99_FP_MACROS_DYNAMIC
#define _GLIBCXX_USE_C99_FP_MACROS_DYNAMIC_WORKAROUND_GUARD 1
#else
#define _GLIBCXX_USE_C99_FP_MACROS_DYNAMIC_WORKAROUND_GUARD 0
#endif

#ifndef _CPPLIB_VER
#define _CPPLIB_VER_WORKAROUND_GUARD 1
#else
#define _CPPLIB_VER_WORKAROUND_GUARD 0
#endif

#ifndef AUTODDS_CLANG_VERSION
#define AUTODDS_CLANG_VERSION_WORKAROUND_GUARD 1
#else
#define AUTODDS_CLANG_VERSION_WORKAROUND_GUARD 0
#endif

#define AUTODDS_WORKAROUND(symbol, test)                \
       ((symbol ## _WORKAROUND_GUARD + 0 == 0) &&     \
       (symbol != 0) && (1 % (( (symbol test) ) + 1)))

#ifdef AUTODDS_DETECT_OUTDATED_WORKAROUNDS
#  define AUTODDS_OPEN_PAREN (
#  define AUTODDS_TESTED_AT(value)  > value) ?(-1): AUTODDS_OPEN_PAREN 1
#else
#  define AUTODDS_TESTED_AT(value) != ((value)-(value))
#endif

#else

#define AUTODDS_WORKAROUND(symbol, test) 0

#endif


#endif //AUTODDS_LIBS_CONFIG_WORKAROUND_HPP_