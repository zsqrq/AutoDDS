//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_DETAIL_STDLIB_SELECTION_CONFIG_HPP_
#define AUTODDS_LIBS_CONFIG_DETAIL_STDLIB_SELECTION_CONFIG_HPP_

#if defined(__cplusplus) && defined(__has_include)
#  if __has_include(<version>)
// It should be safe to include `<version>` when it is present without checking
// the actual C++ language version as it consists solely of macro definitions.
// [version.syn] p1: The header <version> supplies implementation-dependent
// information about the C++ standard library (e.g., version number and release date).
#    include <version>
#  else
#    include <cstddef>
#  endif
#elif defined(__cplusplus)
#  include <cstddef>
#else
#  include <stddef.h>
#endif

#if defined(__GLIBCPP__) || defined(__GLIBCXX__)
#define AUTODDS_STDLIB_CONFIG "libs/config/stdlib/libstdcpp3.hpp"
#endif

#endif //AUTODDS_LIBS_CONFIG_DETAIL_STDLIB_SELECTION_CONFIG_HPP_