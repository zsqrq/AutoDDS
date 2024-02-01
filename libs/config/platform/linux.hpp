//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_PLATFORM_LINUX_HPP_
#define AUTODDS_LIBS_CONFIG_PLATFORM_LINUX_HPP_

#define AUTODDS_PLATFORM "linux"

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#if defined(__GLIBC__) && ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 1)))
#  if defined(__GNUC__) || ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 5)))
#    define AUTODDS_HAS_STDINT_H
#  endif
#endif

#if defined(__GLIBC__) && (__GLIBC__ >= 2)
#  define AUTODDS_HAS_GETTIMEOFDAY
#endif

#ifdef __USE_POSIX199309
#  define AUTODDS_HAS_NANOSLEEP
#endif

#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
// __GLIBC_PREREQ is available since 2.1.2

// swprintf is available since glibc 2.2.0
#  if !__GLIBC_PREREQ(2,2) || (!defined(__USE_ISOC99) && !defined(__USE_UNIX98))
#    define AUTODDS_NO_SWPRINTF
#  endif
#else
#  define AUTODDS_NO_SWPRINTF
#endif

#define AUTODDS_HAS_UNISTD_H
#include "libs/config/detail/posix_features.hpp"
#if defined(__USE_GNU) && !defined(__ANDROID__) && !defined(ANDROID)
#define AUTODDS_HAS_PTHREAD_YIELD
#endif

#ifndef __GNUC__
//
// if the compiler is not gcc we still need to be able to parse
// the GNU system headers, some of which (mainly <stdint.h>)
// use GNU specific extensions:
//
#  ifndef __extension__
#     define __extension__
#  endif
#  ifndef __const__
#     define __const__ const
#  endif
#  ifndef __volatile__
#     define __volatile__ volatile
#  endif
#  ifndef __signed__
#     define __signed__ signed
#  endif
#  ifndef __typeof__
#     define __typeof__ typeof
#  endif
#  ifndef __inline__
#     define __inline__ inline
#  endif
#endif


#endif //AUTODDS_LIBS_CONFIG_PLATFORM_LINUX_HPP_