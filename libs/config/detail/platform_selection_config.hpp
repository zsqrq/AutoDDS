//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_DETAIL_PLATFORM_SELECTION_CONFIG_HPP_
#define AUTODDS_LIBS_CONFIG_DETAIL_PLATFORM_SELECTION_CONFIG_HPP_

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define AUTODDS_PLATFORM_CONFIG "libs/config/platform/linux.hpp"
#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef AUTODDS_HAS_UNISTD_H
#     define AUTODDS_HAS_UNISTD_H
#  endif

#  include "libs/config/detail/posix_features.hpp"

#  endif

#  if defined (AUTODDS_ASSERT_CONFIG)
      // this must come last - generate an error if we don't
      // recognise the platform:
#     error "Unknown platform - please configure and report the results to AUTODDS.org"
#  endif

#endif

#endif //AUTODDS_LIBS_CONFIG_DETAIL_PLATFORM_SELECTION_CONFIG_HPP_