//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_COMMON_CSTDINT_HPP_
#define AUTODDS_LIBS_COMMON_CSTDINT_HPP_

#ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
#endif

#include "libs/config/config.hpp"

#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

#if defined(AUTODDS_HAS_STDINT_H)            \
  && (!defined(__GLIBC__)                  \
      || defined(__GLIBC_HAVE_LONG_LONG)   \
      || (defined(__GLIBC__) && ((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 17)))))
#   include <stdint.h>
#   endif

namespace autodds {
namespace libs {
using ::int8_t;
using ::int_least8_t;
using ::int_fast8_t;
using ::uint8_t;
using ::uint_least8_t;
using ::uint_fast8_t;

using ::int16_t;
using ::int_least16_t;
using ::int_fast16_t;
using ::uint16_t;
using ::uint_least16_t;
using ::uint_fast16_t;

using ::int32_t;
using ::int_least32_t;
using ::int_fast32_t;
using ::uint32_t;
using ::uint_least32_t;
using ::uint_fast32_t;

# ifndef AUTODDS_NO_INT64_T

using ::int64_t;
using ::int_least64_t;
using ::int_fast64_t;
using ::uint64_t;
using ::uint_least64_t;
using ::uint_fast64_t;

# endif

using ::intmax_t;
using ::uintmax_t;
}
}
#if defined(__GNUC__) && defined(__INTPTR_TYPE__) && defined(__UINTPTR_TYPE__)

namespace autodds {
namespace libs {
typedef __INTPTR_TYPE__ intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;
}
}
#define AUTODDS_HAS_INTPTR_T

#endif


#endif //AUTODDS_LIBS_COMMON_CSTDINT_HPP_