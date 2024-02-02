//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_WORKAROUND_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_WORKAROUND_HPP_

#include "libs/config/config.hpp"
#include <cstddef>

#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if    !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES)
#define AUTODDS_MOVE_PERFECT_FORWARDING
#endif

#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ < 5) && !defined(__clang__)
# define AUTODDS_MOVE_OLD_RVALUE_REF_BINDING_RULES
#endif

#if defined(AUTODDS_MOVE_DISABLE_FORCEINLINE)
#define AUTODDS_MOVE_FORCEINLINE inline
#elif defined(AUTODDS_MOVE_FORCEINLINE_IS_AUTODDS_FORCELINE)
#define AUTODDS_MOVE_FORCEINLINE AUTODDS_FORCEINLINE
#elif defined(AUTODDS_GCC) && (__GNUC__ <= 5)
//Older GCCs have problems with forceinline
   #define AUTODDS_MOVE_FORCEINLINE inline
#else
#define AUTODDS_MOVE_FORCEINLINE AUTODDS_FORCEINLINE
#endif

namespace autodds {
namespace libs {
namespace movelib {

template <typename T1>
AUTODDS_FORCEINLINE AUTODDS_CONSTEXPR void ignore(T1 const&)
{}}}}

#if !(defined AUTODDS_NO_EXCEPTIONS)
#    define AUTODDS_MOVE_TRY { try
#    define AUTODDS_MOVE_CATCH(x) catch(x)
#    define AUTODDS_MOVE_RETHROW throw;
#    define AUTODDS_MOVE_CATCH_END }
#endif

#ifndef AUTODDS_NO_CXX11_STATIC_ASSERT
#  ifndef AUTODDS_NO_CXX11_VARIADIC_MACROS
#     define AUTODDS_MOVE_STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)
#  else
#     define AUTODDS_MOVE_STATIC_ASSERT( B ) static_assert(B, #B)
#  endif
#else
namespace autodds::libs {
namespace move_detail {

template<bool B>
struct STATIC_ASSERTION_FAILURE;

template<>
struct STATIC_ASSERTION_FAILURE<true>{};

template<unsigned> struct static_assert_test {};

}}

#define AUTODDS_MOVE_STATIC_ASSERT(B) \
         typedef ::autodds::libs::move_detail::static_assert_test<\
            (unsigned)sizeof(::autodds::libs::move_detail::STATIC_ASSERTION_FAILURE<bool(B)>)>\
               AUTODDS_JOIN(autodds::libs, __LINE__) AUTODDS_ATTRIBUTE_UNUSED

#endif
#define BOOST_MOVE_INTRINSIC_CAST BOOST_MOVE_FORCEINLINE

#endif //AUTODDS_LIBS_MOVE_DETAIL_WORKAROUND_HPP_