//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_WORKAROUND_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_WORKAROUND_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES)\
    && !defined(AUTODDS_INTERPROCESS_DISABLE_VARIADIC_TMPL)
#define AUTODDS_CONTAINER_PERFECT_FORWARDING
#endif

#if !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES) && defined(__GXX_EXPERIMENTAL_CXX0X__)\
    && (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ < 40700)
#define AUTODDS_CONTAINER_UNIMPLEMENTED_PACK_EXPANSION_TO_FIXED_LIST
#endif

#if defined(AUTODDS_GCC_VERSION)
#  if (AUTODDS_GCC_VERSION < 40700) || !defined(AUTODDS_GCC_CXX11)
#     define AUTODDS_CONTAINER_NO_CXX11_DELEGATING_CONSTRUCTORS
#  endif
#endif

#if !defined(AUTODDS_NO_CXX11_HDR_TUPLE) || (defined(AUTODDS_MSVC) && (AUTODDS_MSVC == 1700 || AUTODDS_MSVC == 1600))
#define AUTODDS_CONTAINER_PAIR_TEST_HAS_HEADER_TUPLE
#endif

#ifndef AUTODDS_CONTAINER_MEMZEROED_FLOATING_POINT_IS_NOT_ZERO
#define AUTODDS_CONTAINER_MEMZEROED_FLOATING_POINT_IS_ZERO 1
#endif

#ifndef AUTODDS_CONTAINER_MEMZEROED_POINTER_IS_NOT_NULL
#define AUTODDS_CONTAINER_MEMZEROED_POINTER_IS_NULL
#endif

#define AUTODDS_CONTAINER_DOC1ST(TYPE1, TYPE2) TYPE2
#define AUTODDS_CONTAINER_I ,
#define AUTODDS_CONTAINER_DOCIGN(T) T
#define AUTODDS_CONTAINER_DOCONLY(T)

#if defined(AUTODDS_ALL_DYN_LINK) || defined(AUTODDS_CONTAINER_DYN_LINK)

/* export if this is our own source, otherwise import: */
   #ifdef AUTODDS_CONTAINER_SOURCE
   #  define AUTODDS_CONTAINER_DECL AUTODDS_SYMBOL_EXPORT
   #else
   #  define AUTODDS_CONTAINER_DECL AUTODDS_SYMBOL_IMPORT
   
   #endif  /* AUTODDS_CONTAINER_SOURCE */
#else
#define AUTODDS_CONTAINER_DECL
#endif  /* DYN_LINK */

#if defined(AUTODDS_CONTAINER_DISABLE_FORCEINLINE)
#define AUTODDS_CONTAINER_FORCEINLINE inline
#elif defined(AUTODDS_CONTAINER_FORCEINLINE_IS_AUTODDS_FORCELINE)
#define AUTODDS_CONTAINER_FORCEINLINE AUTODDS_FORCEINLINE
#elif defined(AUTODDS_MSVC) && (_MSC_VER <= 1900 || defined(_DEBUG))
//"__forceinline" and MSVC seems to have some bugs in old versions and in debug mode
   #define AUTODDS_CONTAINER_FORCEINLINE inline
#elif defined(AUTODDS_GCC) && ((__GNUC__ <= 5) || defined(__MINGW32__))
//Older GCCs and MinGw have problems with forceinline
   #define AUTODDS_CONTAINER_FORCEINLINE inline
#else
#define AUTODDS_CONTAINER_FORCEINLINE AUTODDS_FORCEINLINE
#endif

#if defined(AUTODDS_CONTAINER_DISABLE_NOINLINE)
#define AUTODDS_CONTAINER_NOINLINE
#else
#define AUTODDS_CONTAINER_NOINLINE AUTODDS_NOINLINE
#endif

#if !defined(__has_feature)
#define AUTODDS_CONTAINER_HAS_FEATURE(feature) 0
#else
#define AUTODDS_CONTAINER_HAS_FEATURE(feature) __has_feature(feature)
#endif

//Detect address sanitizer
#if defined(__SANITIZE_ADDRESS__) || AUTODDS_CONTAINER_HAS_FEATURE(address_sanitizer)
#define AUTODDS_CONTAINER_ASAN
#endif


#if (AUTODDS_CXX_VERSION < 201703L) || !defined(__cpp_deduction_guides)
#define AUTODDS_CONTAINER_NO_CXX17_CTAD
#endif

#if defined(AUTODDS_CONTAINER_DISABLE_ATTRIBUTE_NODISCARD)
#define AUTODDS_CONTAINER_ATTRIBUTE_NODISCARD
#else
#if   defined(AUTODDS_GCC) && ((AUTODDS_GCC < 100000) || (__cplusplus < 201703L))
//Avoid using it in C++ < 17 and GCC < 10 because it warns in SFINAE contexts
      //(see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89070)
      #define AUTODDS_CONTAINER_ATTRIBUTE_NODISCARD
#else
#define AUTODDS_CONTAINER_ATTRIBUTE_NODISCARD AUTODDS_ATTRIBUTE_NODISCARD
#endif
#endif

namespace autodds {
namespace libs {
namespace container {

template <typename T1>
AUTODDS_FORCEINLINE AUTODDS_CXX14_CONSTEXPR void ignore(T1 const&)
{}

} // namespace container
} // namesoace libs
} // namespace autodds

#if !(defined AUTODDS_NO_EXCEPTIONS)
#    define AUTODDS_CONTAINER_TRY { try
#    define AUTODDS_CONTAINER_CATCH(x) catch(x)
#    define AUTODDS_CONTAINER_RETHROW throw;
#    define AUTODDS_CONTAINER_CATCH_END }
#endif

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_WORKAROUND_HPP_