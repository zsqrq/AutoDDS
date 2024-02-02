//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_COMMON_STATIC_ASSERT_HPP_
#define AUTODDS_LIBS_COMMON_STATIC_ASSERT_HPP_
#include "libs/config/config.hpp"
#include "libs/config/workaround.hpp"
#include <cstddef>

#if defined(__GNUC__) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#pragma GCC system_header
#endif

#ifndef AUTODDS_NO_CXX11_STATIC_ASSERT
#  ifndef AUTODDS_NO_CXX11_VARIADIC_MACROS
#     define AUTODDS_STATIC_ASSERT_MSG( ... ) static_assert(__VA_ARGS__)
#  else
#     define AUTODDS_STATIC_ASSERT_MSG( B, Msg ) static_assert( B, Msg )
#  endif
#else
#     define AUTODDS_STATIC_ASSERT_MSG( B, Msg ) AUTODDS_STATIC_ASSERT( B )
#endif

#if defined(__GNUC__) && (__GNUC__ == 3) && ((__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4))
// gcc 3.3 and 3.4 don't produce good error messages with the default version:
#  define AUTODDS_SA_GCC_WORKAROUND
#endif

#if defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
#  ifndef AUTODDS_NO_CXX11_VARIADIC_MACROS
#     define AUTODDS_STATIC_ASSERT_BOOL_CAST( ... ) ((__VA_ARGS__) != 0)
#  else
#     define AUTODDS_STATIC_ASSERT_BOOL_CAST( x ) ((x) != 0)
#  endif
#else
#  ifndef AUTODDS_NO_CXX11_VARIADIC_MACROS
#     define AUTODDS_STATIC_ASSERT_BOOL_CAST( ... ) (bool)(__VA_ARGS__)
#  else
#     define AUTODDS_STATIC_ASSERT_BOOL_CAST(x) (bool)(x)
#  endif
#endif

//#ifndef AUTODDS_NO_CXX11_STATIC_ASSERT
//#  ifndef AUTODDS_NO_CXX11_VARIADIC_MACROS
//#     define AUTODDS_STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)
//#  else
//#     define AUTODDS_STATIC_ASSERT( B ) static_assert(B, #B)
//#  endif
//#else
//
//#endif

#endif //AUTODDS_LIBS_COMMON_STATIC_ASSERT_HPP_