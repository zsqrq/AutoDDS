//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_DETAIL_SUFFIX_HPP_
#define AUTODDS_LIBS_CONFIG_DETAIL_SUFFIX_HPP_

#if defined(__GNUC__) && (__GNUC__ >= 4)
//
// Some GCC-4.x versions issue warnings even when __extension__ is used,
// so use this as a workaround:
//
#pragma GCC system_header
#endif

//
// ensure that visibility macros are always defined, thus simplifying use
//
#ifndef AUTODDS_SYMBOL_EXPORT
# define AUTODDS_SYMBOL_EXPORT
#endif
#ifndef AUTODDS_SYMBOL_IMPORT
# define AUTODDS_SYMBOL_IMPORT
#endif
#ifndef AUTODDS_SYMBOL_VISIBLE
# define AUTODDS_SYMBOL_VISIBLE
#endif

//
// disable explicitly enforced visibility
//
#if defined(AUTODDS_DISABLE_EXPLICIT_SYMBOL_VISIBILITY)

#undef AUTODDS_SYMBOL_EXPORT
#define AUTODDS_SYMBOL_EXPORT

#undef AUTODDS_SYMBOL_IMPORT
#define AUTODDS_SYMBOL_IMPORT

#undef AUTODDS_SYMBOL_VISIBLE
#define AUTODDS_SYMBOL_VISIBLE

#endif

#if !defined(AUTODDS_HAS_LONG_LONG) && !defined(AUTODDS_NO_LONG_LONG)                                              \
   && !defined(AUTODDS_MSVC) && !defined(AUTODDS_BORLANDC)
# include <limits.h>
# if (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX))
#   define AUTODDS_HAS_LONG_LONG
# else
#   define AUTODDS_NO_LONG_LONG
# endif
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(AUTODDS_NO_CTYPE_FUNCTIONS)
#  undef AUTODDS_NO_CTYPE_FUNCTIONS
#endif

#  ifndef AUTODDS_STD_EXTENSION_NAMESPACE
#    define AUTODDS_STD_EXTENSION_NAMESPACE std
#  endif

#  if defined(AUTODDS_NO_CV_SPECIALIZATIONS) \
      && !defined(AUTODDS_NO_CV_VOID_SPECIALIZATIONS)
#     define AUTODDS_NO_CV_VOID_SPECIALIZATIONS
#  endif

#  if defined(AUTODDS_NO_LIMITS) \
      && !defined(AUTODDS_NO_LIMITS_COMPILE_TIME_CONSTANTS)
#     define AUTODDS_NO_LIMITS_COMPILE_TIME_CONSTANTS
#     define AUTODDS_NO_MS_INT64_NUMERIC_LIMITS
#     define AUTODDS_NO_LONG_LONG_NUMERIC_LIMITS
#  endif

#if !defined(AUTODDS_HAS_LONG_LONG) && !defined(AUTODDS_NO_LONG_LONG_NUMERIC_LIMITS)
#  define AUTODDS_NO_LONG_LONG_NUMERIC_LIMITS
#endif

#if !defined(AUTODDS_HAS_MS_INT64) && !defined(AUTODDS_NO_MS_INT64_NUMERIC_LIMITS)
#  define AUTODDS_NO_MS_INT64_NUMERIC_LIMITS
#endif

#  if !defined(AUTODDS_NO_MEMBER_TEMPLATES) \
       && !defined(AUTODDS_MSVC6_MEMBER_TEMPLATES)
#     define AUTODDS_MSVC6_MEMBER_TEMPLATES
#  endif

#  if defined(AUTODDS_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(AUTODDS_BCB_PARTIAL_SPECIALIZATION_BUG)
#     define AUTODDS_BCB_PARTIAL_SPECIALIZATION_BUG
#  endif

#  if defined(AUTODDS_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(AUTODDS_NO_ARRAY_TYPE_SPECIALIZATIONS)
#     define AUTODDS_NO_ARRAY_TYPE_SPECIALIZATIONS
#  endif

#  if defined(AUTODDS_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(AUTODDS_NO_STD_ITERATOR_TRAITS)
#     define AUTODDS_NO_STD_ITERATOR_TRAITS
#  endif

#  if defined(AUTODDS_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(AUTODDS_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
#     define AUTODDS_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#  endif

#  if defined(AUTODDS_NO_MEMBER_TEMPLATES) \
      && !defined(AUTODDS_MSVC6_MEMBER_TEMPLATES) \
      && !defined(AUTODDS_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#     define AUTODDS_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  endif

#  if defined(AUTODDS_NO_MEMBER_TEMPLATES) \
      && !defined(AUTODDS_MSVC6_MEMBER_TEMPLATES) \
      && !defined(AUTODDS_NO_STD_ALLOCATOR)
#     define AUTODDS_NO_STD_ALLOCATOR
#  endif

#if defined(AUTODDS_NO_ARGUMENT_DEPENDENT_LOOKUP) && !defined(AUTODDS_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
#  define AUTODDS_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#endif

#if defined(AUTODDS_NO_TYPEID) && !defined(AUTODDS_NO_RTTI)
#  define AUTODDS_NO_RTTI
#endif

#if !defined(AUTODDS_NO_STD_ALLOCATOR)
#  define AUTODDS_HAS_PARTIAL_STD_ALLOCATOR
#endif

#  if defined(AUTODDS_NO_STD_LOCALE) && !defined(AUTODDS_NO_STD_USE_FACET)
#     define AUTODDS_NO_STD_USE_FACET
#  endif

#  if defined(AUTODDS_NO_STD_LOCALE) && !defined(AUTODDS_NO_STD_MESSAGES)
#     define AUTODDS_NO_STD_MESSAGES
#  endif

#  if defined(AUTODDS_NO_STD_LOCALE) && !defined(AUTODDS_NO_STD_WSTREAMBUF)
#     define AUTODDS_NO_STD_WSTREAMBUF
#  endif

#  if defined(AUTODDS_NO_CWCHAR) && !defined(AUTODDS_NO_CWCTYPE)
#     define AUTODDS_NO_CWCTYPE
#  endif

#  if defined(AUTODDS_NO_CWCHAR) && !defined(AUTODDS_NO_SWPRINTF)
#     define AUTODDS_NO_SWPRINTF
#  endif

#if defined(AUTODDS_DISABLE_WIN32) && defined(_WIN32) \
   && !defined(AUTODDS_DISABLE_THREADS) && !defined(AUTODDS_HAS_PTHREADS)
#  define AUTODDS_DISABLE_THREADS
#endif

#if (defined(__MT__) || defined(_MT) || defined(_REENTRANT) \
    || defined(_PTHREADS) || defined(__APPLE__) || defined(__DragonFly__)) \
    && !defined(AUTODDS_HAS_THREADS)
#  define AUTODDS_HAS_THREADS
#endif

#if defined(AUTODDS_DISABLE_THREADS) && defined(AUTODDS_HAS_THREADS)
#  undef AUTODDS_HAS_THREADS
#endif

#if defined(AUTODDS_HAS_THREADS) && !defined(AUTODDS_HAS_PTHREADS)\
      && !defined(AUTODDS_HAS_WINTHREADS) && !defined(AUTODDS_HAS_BETHREADS)\
      && !defined(AUTODDS_HAS_MPTASKS)
#  undef AUTODDS_HAS_THREADS
#endif

#ifndef AUTODDS_HAS_THREADS
#  undef AUTODDS_HAS_PTHREADS
#  undef AUTODDS_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  undef AUTODDS_HAS_PTHREAD_YIELD
#  undef AUTODDS_HAS_PTHREAD_DELAY_NP
#  undef AUTODDS_HAS_WINTHREADS
#  undef AUTODDS_HAS_BETHREADS
#  undef AUTODDS_HAS_MPTASKS
#endif

#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#     define AUTODDS_HAS_STDINT_H
#     ifndef AUTODDS_HAS_LOG1P
#        define AUTODDS_HAS_LOG1P
#     endif
#     ifndef AUTODDS_HAS_EXPM1
#        define AUTODDS_HAS_EXPM1
#     endif
#  endif

#  if !defined(AUTODDS_HAS_SLIST) && !defined(AUTODDS_NO_SLIST)
#     define AUTODDS_NO_SLIST
#  endif

#  if !defined(AUTODDS_HAS_HASH) && !defined(AUTODDS_NO_HASH)
#     define AUTODDS_NO_HASH
#  endif

#if defined(AUTODDS_HAS_SLIST) && !defined(AUTODDS_SLIST_HEADER)
#  define AUTODDS_SLIST_HEADER <slist>
#endif

#if defined(AUTODDS_HAS_HASH) && !defined(AUTODDS_HASH_SET_HEADER)
#  define AUTODDS_HASH_SET_HEADER <hash_set>
#endif

#if defined(AUTODDS_HAS_HASH) && !defined(AUTODDS_HASH_MAP_HEADER)
#  define AUTODDS_HASH_MAP_HEADER <hash_map>
#endif

#if defined(AUTODDS_ABI_PREFIX) && defined(AUTODDS_ABI_SUFFIX) && !defined(AUTODDS_HAS_ABI_HEADERS)
#  define AUTODDS_HAS_ABI_HEADERS
#endif

#if defined(AUTODDS_HAS_ABI_HEADERS) && defined(AUTODDS_DISABLE_ABI_HEADERS)
#  undef AUTODDS_HAS_ABI_HEADERS
#endif

# if defined(AUTODDS_NO_STDC_NAMESPACE) && defined(__cplusplus)
#   include <cstddef>
    namespace std { using ::ptrdiff_t; using ::size_t; }
# endif

#define AUTODDS_PREVENT_MACRO_SUBSTITUTION

#ifndef AUTODDS_USING_STD_MIN
#  define AUTODDS_USING_STD_MIN() using std::min
#endif

#ifndef AUTODDS_USING_STD_MAX
#  define AUTODDS_USING_STD_MAX() using std::max
#endif

#  if defined(AUTODDS_NO_STD_MIN_MAX) && defined(__cplusplus)

namespace std {
  template <class _Tp>
  inline const _Tp& min AUTODDS_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
  }
  template <class _Tp>
  inline const _Tp& max AUTODDS_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return  __a < __b ? __b : __a;
  }
}

#  endif

#  ifdef AUTODDS_NO_INCLASS_MEMBER_INITIALIZATION
#       define AUTODDS_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define AUTODDS_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

#if defined(AUTODDS_NO_STD_USE_FACET)
#  ifdef AUTODDS_HAS_TWO_ARG_USE_FACET
#     define AUTODDS_USE_FACET(Type, loc) std::use_facet(loc, static_cast<Type*>(0))
#     define AUTODDS_HAS_FACET(Type, loc) std::has_facet(loc, static_cast<Type*>(0))
#  elif defined(AUTODDS_HAS_MACRO_USE_FACET)
#     define AUTODDS_USE_FACET(Type, loc) std::_USE(loc, Type)
#     define AUTODDS_HAS_FACET(Type, loc) std::_HAS(loc, Type)
#  elif defined(AUTODDS_HAS_STLP_USE_FACET)
#     define AUTODDS_USE_FACET(Type, loc) (*std::_Use_facet<Type >(loc))
#     define AUTODDS_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#  endif
#else
#  define AUTODDS_USE_FACET(Type, loc) std::use_facet< Type >(loc)
#  define AUTODDS_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#endif

#ifndef AUTODDS_NO_MEMBER_TEMPLATE_KEYWORD
#  define AUTODDS_NESTED_TEMPLATE template
#else
#  define AUTODDS_NESTED_TEMPLATE
#endif

#ifndef AUTODDS_UNREACHABLE_RETURN
#  ifdef AUTODDS_NO_UNREACHABLE_RETURN_DETECTION
#     define AUTODDS_UNREACHABLE_RETURN(x) return x;
#  else
#     define AUTODDS_UNREACHABLE_RETURN(x)
#  endif
#endif

#ifndef AUTODDS_NO_DEDUCED_TYPENAME
#  define AUTODDS_DEDUCED_TYPENAME typename
#else
#  define AUTODDS_DEDUCED_TYPENAME
#endif

#ifndef AUTODDS_NO_TYPENAME_WITH_CTOR
#  define AUTODDS_CTOR_TYPENAME typename
#else
#  define AUTODDS_CTOR_TYPENAME
#endif

#if defined(__CUDA_ARCH__) && defined(AUTODDS_HAS_FLOAT128)
#  undef AUTODDS_HAS_FLOAT128
#endif
#if defined(__CUDA_ARCH__) && defined(AUTODDS_HAS_INT128)
#  undef AUTODDS_HAS_INT128
#endif

#if defined(AUTODDS_HAS_LONG_LONG) && defined(__cplusplus)
namespace autodds{
#  ifdef __GNUC__
   __extension__ typedef long long long_long_type;
   __extension__ typedef unsigned long long ulong_long_type;
#  else
   typedef long long long_long_type;
   typedef unsigned long long ulong_long_type;
#  endif
}
#endif

#if defined(AUTODDS_HAS_INT128) && defined(__cplusplus)
namespace autodds{
#  ifdef __GNUC__
   __extension__ typedef __int128 int128_type;
   __extension__ typedef unsigned __int128 uint128_type;
#  else
   typedef __int128 int128_type;
   typedef unsigned __int128 uint128_type;
#  endif
}
#endif

#if defined(AUTODDS_HAS_FLOAT128) && defined(__cplusplus)
namespace autodds {
#  ifdef __GNUC__
   __extension__ typedef __float128 float128_type;
#  else
   typedef __float128 float128_type;
#  endif
}
#endif

// These macros are obsolete. Port away and remove.
#  define AUTODDS_EXPLICIT_TEMPLATE_TYPE(t)
#  define AUTODDS_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define AUTODDS_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define AUTODDS_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#  define AUTODDS_APPEND_EXPLICIT_TEMPLATE_TYPE(t)
#  define AUTODDS_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define AUTODDS_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define AUTODDS_APPEND_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#if defined(AUTODDS_NO_STD_TYPEINFO) && defined(__cplusplus) && defined(AUTODDS_MSVC)
#include <typeinfo>
namespace std{ using ::type_info; }
// Since we do now have typeinfo, undef the macro:
#undef AUTODDS_NO_STD_TYPEINFO
#endif

#include "libs/config/helper_macros.hpp"

#  ifndef AUTODDS_COMPILER
#     define AUTODDS_COMPILER "Unknown ISO C++ Compiler"
#  endif
#  ifndef AUTODDS_STDLIB
#     define AUTODDS_STDLIB "Unknown ISO standard library"
#  endif
#  ifndef AUTODDS_PLATFORM
#     if defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) \
         || defined(_POSIX_SOURCE)
#        define AUTODDS_PLATFORM "Generic Unix"
#     else
#        define AUTODDS_PLATFORM "Unknown"
#     endif
#  endif

#  ifndef AUTODDS_GPU_ENABLED
#  define AUTODDS_GPU_ENABLED
#  endif

#if !defined(AUTODDS_RESTRICT)
#  if defined(_MSC_VER)
#    define AUTODDS_RESTRICT __restrict
#    if !defined(AUTODDS_NO_RESTRICT_REFERENCES) && (_MSC_FULL_VER < 190023026)
#      define AUTODDS_NO_RESTRICT_REFERENCES
#    endif
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define AUTODDS_RESTRICT __restrict__
#  else
#    define AUTODDS_RESTRICT
#    if !defined(AUTODDS_NO_RESTRICT_REFERENCES)
#      define AUTODDS_NO_RESTRICT_REFERENCES
#    endif
#  endif
#endif

#if !defined(AUTODDS_MAY_ALIAS)
#  define AUTODDS_NO_MAY_ALIAS
#  define AUTODDS_MAY_ALIAS
#endif

#if !defined(AUTODDS_FORCEINLINE)
#  if defined(_MSC_VER)
#    define AUTODDS_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define AUTODDS_FORCEINLINE inline __attribute__ ((__always_inline__))
#  else
#    define AUTODDS_FORCEINLINE inline
#  endif
#endif

#if !defined(AUTODDS_NOINLINE)
#  if defined(_MSC_VER)
#    define AUTODDS_NOINLINE __declspec(noinline)
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    if defined(__CUDACC__)
#      define AUTODDS_NOINLINE __attribute__ ((noinline))
#    elif defined(__HIP__)
#      define AUTODDS_NOINLINE __attribute__ ((noinline))
#    else
#      define AUTODDS_NOINLINE __attribute__ ((__noinline__))
#    endif
#  else
#    define AUTODDS_NOINLINE
#  endif
#endif

#if !defined(AUTODDS_NORETURN)
#  if defined(_MSC_VER)
#    define AUTODDS_NORETURN __declspec(noreturn)
#  elif defined(__GNUC__) || defined(__CODEGEARC__) && defined(__clang__)
#    define AUTODDS_NORETURN __attribute__ ((__noreturn__))
#  elif defined(__has_attribute) && defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x5130)
#    if __has_attribute(noreturn)
#      define AUTODDS_NORETURN [[noreturn]]
#    endif
#  elif defined(__has_cpp_attribute)
#    if __has_cpp_attribute(noreturn)
#      define AUTODDS_NORETURN [[noreturn]]
#    endif
#  endif
#endif

#if !defined(AUTODDS_NORETURN)
#  define AUTODDS_NO_NORETURN
#  define AUTODDS_NORETURN
#endif

#if !defined(AUTODDS_DEPRECATED) && __cplusplus >= 201402
#define AUTODDS_DEPRECATED(msg) [[deprecated(msg)]]
#endif

#if defined(AUTODDS_ALLOW_DEPRECATED_SYMBOLS) || defined(AUTODDS_ALLOW_DEPRECATED)
#undef AUTODDS_DEPRECATED
#endif

#if !defined(AUTODDS_DEPRECATED)
#define AUTODDS_DEPRECATED(msg)
#endif

#if !defined(AUTODDS_LIKELY)
#  define AUTODDS_LIKELY(x) x
#endif
#if !defined(AUTODDS_UNLIKELY)
#  define AUTODDS_UNLIKELY(x) x
#endif

#if !defined(AUTODDS_NO_CXX11_OVERRIDE)
#  define AUTODDS_OVERRIDE override
#else
#  define AUTODDS_OVERRIDE
#endif

#if !defined(AUTODDS_ALIGNMENT)
#  if !defined(AUTODDS_NO_CXX11_ALIGNAS)
#    define AUTODDS_ALIGNMENT(x) alignas(x)
#  elif defined(_MSC_VER)
#    define AUTODDS_ALIGNMENT(x) __declspec(align(x))
#  elif defined(__GNUC__)
#    define AUTODDS_ALIGNMENT(x) __attribute__ ((__aligned__(x)))
#  else
#    define AUTODDS_NO_ALIGNMENT
#    define AUTODDS_ALIGNMENT(x)
#  endif
#endif

#if !defined(AUTODDS_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS) && defined(AUTODDS_NO_CXX11_DEFAULTED_FUNCTIONS)
#  define AUTODDS_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS
#endif

#if !defined(AUTODDS_NO_CXX11_DEFAULTED_MOVES) && (defined(AUTODDS_NO_CXX11_DEFAULTED_FUNCTIONS) || defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES))
#  define AUTODDS_NO_CXX11_DEFAULTED_MOVES
#endif

#if !(defined(AUTODDS_NO_CXX11_DEFAULTED_FUNCTIONS) || defined(AUTODDS_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS))
#   define AUTODDS_DEFAULTED_FUNCTION(fun, body) fun = default;
#else
#   define AUTODDS_DEFAULTED_FUNCTION(fun, body) fun body
#endif

#if !defined(AUTODDS_NO_CXX11_DELETED_FUNCTIONS)
#   define AUTODDS_DELETED_FUNCTION(fun) fun = delete;
#else
#   define AUTODDS_DELETED_FUNCTION(fun) private: fun;
#endif

#if defined(AUTODDS_NO_CXX11_DECLTYPE) && !defined(AUTODDS_NO_CXX11_DECLTYPE_N3276)
#define AUTODDS_NO_CXX11_DECLTYPE_N3276 AUTODDS_NO_CXX11_DECLTYPE
#endif

#if defined(AUTODDS_NO_CXX11_HDR_UNORDERED_MAP) || defined (AUTODDS_NO_CXX11_HDR_UNORDERED_SET)
# ifndef AUTODDS_NO_CXX11_STD_UNORDERED
#  define AUTODDS_NO_CXX11_STD_UNORDERED
# endif
#endif

//  Use AUTODDS_NO_CXX11_HDR_INITIALIZER_LIST instead of AUTODDS_NO_INITIALIZER_LISTS
#if defined(AUTODDS_NO_CXX11_HDR_INITIALIZER_LIST) && !defined(AUTODDS_NO_INITIALIZER_LISTS)
#  define AUTODDS_NO_INITIALIZER_LISTS
#endif

//  Use AUTODDS_NO_CXX11_HDR_ARRAY instead of AUTODDS_NO_0X_HDR_ARRAY
#if defined(AUTODDS_NO_CXX11_HDR_ARRAY) && !defined(AUTODDS_NO_0X_HDR_ARRAY)
#  define AUTODDS_NO_0X_HDR_ARRAY
#endif
//  Use AUTODDS_NO_CXX11_HDR_CHRONO instead of AUTODDS_NO_0X_HDR_CHRONO
#if defined(AUTODDS_NO_CXX11_HDR_CHRONO) && !defined(AUTODDS_NO_0X_HDR_CHRONO)
#  define AUTODDS_NO_0X_HDR_CHRONO
#endif
//  Use AUTODDS_NO_CXX11_HDR_CODECVT instead of AUTODDS_NO_0X_HDR_CODECVT
#if defined(AUTODDS_NO_CXX11_HDR_CODECVT) && !defined(AUTODDS_NO_0X_HDR_CODECVT)
#  define AUTODDS_NO_0X_HDR_CODECVT
#endif
//  Use AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE instead of AUTODDS_NO_0X_HDR_CONDITION_VARIABLE
#if defined(AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE) && !defined(AUTODDS_NO_0X_HDR_CONDITION_VARIABLE)
#  define AUTODDS_NO_0X_HDR_CONDITION_VARIABLE
#endif
//  Use AUTODDS_NO_CXX11_HDR_FORWARD_LIST instead of AUTODDS_NO_0X_HDR_FORWARD_LIST
#if defined(AUTODDS_NO_CXX11_HDR_FORWARD_LIST) && !defined(AUTODDS_NO_0X_HDR_FORWARD_LIST)
#  define AUTODDS_NO_0X_HDR_FORWARD_LIST
#endif
//  Use AUTODDS_NO_CXX11_HDR_FUTURE instead of AUTODDS_NO_0X_HDR_FUTURE
#if defined(AUTODDS_NO_CXX11_HDR_FUTURE) && !defined(AUTODDS_NO_0X_HDR_FUTURE)
#  define AUTODDS_NO_0X_HDR_FUTURE
#endif

#ifdef AUTODDS_NO_CXX11_HDR_INITIALIZER_LIST
# ifndef AUTODDS_NO_0X_HDR_INITIALIZER_LIST
#  define AUTODDS_NO_0X_HDR_INITIALIZER_LIST
# endif
# ifndef AUTODDS_NO_INITIALIZER_LISTS
#  define AUTODDS_NO_INITIALIZER_LISTS
# endif
#endif

//  Use AUTODDS_NO_CXX11_HDR_MUTEX instead of AUTODDS_NO_0X_HDR_MUTEX
#if defined(AUTODDS_NO_CXX11_HDR_MUTEX) && !defined(AUTODDS_NO_0X_HDR_MUTEX)
#  define AUTODDS_NO_0X_HDR_MUTEX
#endif
//  Use AUTODDS_NO_CXX11_HDR_RANDOM instead of AUTODDS_NO_0X_HDR_RANDOM
#if defined(AUTODDS_NO_CXX11_HDR_RANDOM) && !defined(AUTODDS_NO_0X_HDR_RANDOM)
#  define AUTODDS_NO_0X_HDR_RANDOM
#endif
//  Use AUTODDS_NO_CXX11_HDR_RATIO instead of AUTODDS_NO_0X_HDR_RATIO
#if defined(AUTODDS_NO_CXX11_HDR_RATIO) && !defined(AUTODDS_NO_0X_HDR_RATIO)
#  define AUTODDS_NO_0X_HDR_RATIO
#endif
//  Use AUTODDS_NO_CXX11_HDR_REGEX instead of AUTODDS_NO_0X_HDR_REGEX
#if defined(AUTODDS_NO_CXX11_HDR_REGEX) && !defined(AUTODDS_NO_0X_HDR_REGEX)
#  define AUTODDS_NO_0X_HDR_REGEX
#endif
//  Use AUTODDS_NO_CXX11_HDR_SYSTEM_ERROR instead of AUTODDS_NO_0X_HDR_SYSTEM_ERROR
#if defined(AUTODDS_NO_CXX11_HDR_SYSTEM_ERROR) && !defined(AUTODDS_NO_0X_HDR_SYSTEM_ERROR)
#  define AUTODDS_NO_0X_HDR_SYSTEM_ERROR
#endif
//  Use AUTODDS_NO_CXX11_HDR_THREAD instead of AUTODDS_NO_0X_HDR_THREAD
#if defined(AUTODDS_NO_CXX11_HDR_THREAD) && !defined(AUTODDS_NO_0X_HDR_THREAD)
#  define AUTODDS_NO_0X_HDR_THREAD
#endif
//  Use AUTODDS_NO_CXX11_HDR_TUPLE instead of AUTODDS_NO_0X_HDR_TUPLE
#if defined(AUTODDS_NO_CXX11_HDR_TUPLE) && !defined(AUTODDS_NO_0X_HDR_TUPLE)
#  define AUTODDS_NO_0X_HDR_TUPLE
#endif
//  Use AUTODDS_NO_CXX11_HDR_TYPE_TRAITS instead of AUTODDS_NO_0X_HDR_TYPE_TRAITS
#if defined(AUTODDS_NO_CXX11_HDR_TYPE_TRAITS) && !defined(AUTODDS_NO_0X_HDR_TYPE_TRAITS)
#  define AUTODDS_NO_0X_HDR_TYPE_TRAITS
#endif
//  Use AUTODDS_NO_CXX11_HDR_TYPEINDEX instead of AUTODDS_NO_0X_HDR_TYPEINDEX
#if defined(AUTODDS_NO_CXX11_HDR_TYPEINDEX) && !defined(AUTODDS_NO_0X_HDR_TYPEINDEX)
#  define AUTODDS_NO_0X_HDR_TYPEINDEX
#endif
//  Use AUTODDS_NO_CXX11_HDR_UNORDERED_MAP instead of AUTODDS_NO_0X_HDR_UNORDERED_MAP
#if defined(AUTODDS_NO_CXX11_HDR_UNORDERED_MAP) && !defined(AUTODDS_NO_0X_HDR_UNORDERED_MAP)
#  define AUTODDS_NO_0X_HDR_UNORDERED_MAP
#endif
//  Use AUTODDS_NO_CXX11_HDR_UNORDERED_SET instead of AUTODDS_NO_0X_HDR_UNORDERED_SET
#if defined(AUTODDS_NO_CXX11_HDR_UNORDERED_SET) && !defined(AUTODDS_NO_0X_HDR_UNORDERED_SET)
#  define AUTODDS_NO_0X_HDR_UNORDERED_SET
#endif

#if defined(AUTODDS_NO_CXX11_AUTO_DECLARATIONS) && !defined(AUTODDS_NO_AUTO_DECLARATIONS)
#  define AUTODDS_NO_AUTO_DECLARATIONS
#endif
//  Use     AUTODDS_NO_CXX11_AUTO_MULTIDECLARATIONS instead of   AUTODDS_NO_AUTO_MULTIDECLARATIONS
#if defined(AUTODDS_NO_CXX11_AUTO_MULTIDECLARATIONS) && !defined(AUTODDS_NO_AUTO_MULTIDECLARATIONS)
#  define AUTODDS_NO_AUTO_MULTIDECLARATIONS
#endif
//  Use     AUTODDS_NO_CXX11_CHAR16_T instead of   AUTODDS_NO_CHAR16_T
#if defined(AUTODDS_NO_CXX11_CHAR16_T) && !defined(AUTODDS_NO_CHAR16_T)
#  define AUTODDS_NO_CHAR16_T
#endif
//  Use     AUTODDS_NO_CXX11_CHAR32_T instead of   AUTODDS_NO_CHAR32_T
#if defined(AUTODDS_NO_CXX11_CHAR32_T) && !defined(AUTODDS_NO_CHAR32_T)
#  define AUTODDS_NO_CHAR32_T
#endif
//  Use     AUTODDS_NO_CXX11_TEMPLATE_ALIASES instead of   AUTODDS_NO_TEMPLATE_ALIASES
#if defined(AUTODDS_NO_CXX11_TEMPLATE_ALIASES) && !defined(AUTODDS_NO_TEMPLATE_ALIASES)
#  define AUTODDS_NO_TEMPLATE_ALIASES
#endif
//  Use     AUTODDS_NO_CXX11_CONSTEXPR instead of   AUTODDS_NO_CONSTEXPR
#if defined(AUTODDS_NO_CXX11_CONSTEXPR) && !defined(AUTODDS_NO_CONSTEXPR)
#  define AUTODDS_NO_CONSTEXPR
#endif
//  Use     AUTODDS_NO_CXX11_DECLTYPE_N3276 instead of   AUTODDS_NO_DECLTYPE_N3276
#if defined(AUTODDS_NO_CXX11_DECLTYPE_N3276) && !defined(AUTODDS_NO_DECLTYPE_N3276)
#  define AUTODDS_NO_DECLTYPE_N3276
#endif
//  Use     AUTODDS_NO_CXX11_DECLTYPE instead of   AUTODDS_NO_DECLTYPE
#if defined(AUTODDS_NO_CXX11_DECLTYPE) && !defined(AUTODDS_NO_DECLTYPE)
#  define AUTODDS_NO_DECLTYPE
#endif
//  Use     AUTODDS_NO_CXX11_DEFAULTED_FUNCTIONS instead of   AUTODDS_NO_DEFAULTED_FUNCTIONS
#if defined(AUTODDS_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(AUTODDS_NO_DEFAULTED_FUNCTIONS)
#  define AUTODDS_NO_DEFAULTED_FUNCTIONS
#endif
//  Use     AUTODDS_NO_CXX11_DELETED_FUNCTIONS instead of   AUTODDS_NO_DELETED_FUNCTIONS
#if defined(AUTODDS_NO_CXX11_DELETED_FUNCTIONS) && !defined(AUTODDS_NO_DELETED_FUNCTIONS)
#  define AUTODDS_NO_DELETED_FUNCTIONS
#endif
//  Use     AUTODDS_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS instead of   AUTODDS_NO_EXPLICIT_CONVERSION_OPERATORS
#if defined(AUTODDS_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS) && !defined(AUTODDS_NO_EXPLICIT_CONVERSION_OPERATORS)
#  define AUTODDS_NO_EXPLICIT_CONVERSION_OPERATORS
#endif
//  Use     AUTODDS_NO_CXX11_EXTERN_TEMPLATE instead of   AUTODDS_NO_EXTERN_TEMPLATE
#if defined(AUTODDS_NO_CXX11_EXTERN_TEMPLATE) && !defined(AUTODDS_NO_EXTERN_TEMPLATE)
#  define AUTODDS_NO_EXTERN_TEMPLATE
#endif
//  Use     AUTODDS_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS instead of   AUTODDS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#if defined(AUTODDS_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) && !defined(AUTODDS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
#  define AUTODDS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#endif
//  Use     AUTODDS_NO_CXX11_LAMBDAS instead of   AUTODDS_NO_LAMBDAS
#if defined(AUTODDS_NO_CXX11_LAMBDAS) && !defined(AUTODDS_NO_LAMBDAS)
#  define AUTODDS_NO_LAMBDAS
#endif
//  Use     AUTODDS_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS instead of   AUTODDS_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#if defined(AUTODDS_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS) && !defined(AUTODDS_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS)
#  define AUTODDS_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#endif
//  Use     AUTODDS_NO_CXX11_NOEXCEPT instead of   AUTODDS_NO_NOEXCEPT
#if defined(AUTODDS_NO_CXX11_NOEXCEPT) && !defined(AUTODDS_NO_NOEXCEPT)
#  define AUTODDS_NO_NOEXCEPT
#endif
//  Use     AUTODDS_NO_CXX11_NULLPTR instead of   AUTODDS_NO_NULLPTR
#if defined(AUTODDS_NO_CXX11_NULLPTR) && !defined(AUTODDS_NO_NULLPTR)
#  define AUTODDS_NO_NULLPTR
#endif
//  Use     AUTODDS_NO_CXX11_RAW_LITERALS instead of   AUTODDS_NO_RAW_LITERALS
#if defined(AUTODDS_NO_CXX11_RAW_LITERALS) && !defined(AUTODDS_NO_RAW_LITERALS)
#  define AUTODDS_NO_RAW_LITERALS
#endif
//  Use     AUTODDS_NO_CXX11_RVALUE_REFERENCES instead of   AUTODDS_NO_RVALUE_REFERENCES
#if defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_NO_RVALUE_REFERENCES)
#  define AUTODDS_NO_RVALUE_REFERENCES
#endif
//  Use     AUTODDS_NO_CXX11_SCOPED_ENUMS instead of   AUTODDS_NO_SCOPED_ENUMS
#if defined(AUTODDS_NO_CXX11_SCOPED_ENUMS) && !defined(AUTODDS_NO_SCOPED_ENUMS)
#  define AUTODDS_NO_SCOPED_ENUMS
#endif
//  Use     AUTODDS_NO_CXX11_STATIC_ASSERT instead of   AUTODDS_NO_STATIC_ASSERT
#if defined(AUTODDS_NO_CXX11_STATIC_ASSERT) && !defined(AUTODDS_NO_STATIC_ASSERT)
#  define AUTODDS_NO_STATIC_ASSERT
#endif
//  Use     AUTODDS_NO_CXX11_STD_UNORDERED instead of   AUTODDS_NO_STD_UNORDERED
#if defined(AUTODDS_NO_CXX11_STD_UNORDERED) && !defined(AUTODDS_NO_STD_UNORDERED)
#  define AUTODDS_NO_STD_UNORDERED
#endif
//  Use     AUTODDS_NO_CXX11_UNICODE_LITERALS instead of   AUTODDS_NO_UNICODE_LITERALS
#if defined(AUTODDS_NO_CXX11_UNICODE_LITERALS) && !defined(AUTODDS_NO_UNICODE_LITERALS)
#  define AUTODDS_NO_UNICODE_LITERALS
#endif
//  Use     AUTODDS_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX instead of   AUTODDS_NO_UNIFIED_INITIALIZATION_SYNTAX
#if defined(AUTODDS_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX) && !defined(AUTODDS_NO_UNIFIED_INITIALIZATION_SYNTAX)
#  define AUTODDS_NO_UNIFIED_INITIALIZATION_SYNTAX
#endif
//  Use     AUTODDS_NO_CXX11_VARIADIC_TEMPLATES instead of   AUTODDS_NO_VARIADIC_TEMPLATES
#if defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES) && !defined(AUTODDS_NO_VARIADIC_TEMPLATES)
#  define AUTODDS_NO_VARIADIC_TEMPLATES
#endif
//  Use     AUTODDS_NO_CXX11_VARIADIC_MACROS instead of   AUTODDS_NO_VARIADIC_MACROS
#if defined(AUTODDS_NO_CXX11_VARIADIC_MACROS) && !defined(AUTODDS_NO_VARIADIC_MACROS)
#  define AUTODDS_NO_VARIADIC_MACROS
#endif
//  Use     AUTODDS_NO_CXX11_NUMERIC_LIMITS instead of   AUTODDS_NO_NUMERIC_LIMITS_LOWEST
#if defined(AUTODDS_NO_CXX11_NUMERIC_LIMITS) && !defined(AUTODDS_NO_NUMERIC_LIMITS_LOWEST)
#  define AUTODDS_NO_NUMERIC_LIMITS_LOWEST
#endif

#if !defined(AUTODDS_NO_CXX11_FINAL)
#  define AUTODDS_FINAL final
#else
#  define AUTODDS_FINAL
#endif

#ifdef AUTODDS_NO_CXX11_NOEXCEPT
#  define AUTODDS_NOEXCEPT
#  define AUTODDS_NOEXCEPT_OR_NOTHROW throw()
#  define AUTODDS_NOEXCEPT_IF(Predicate)
#  define AUTODDS_NOEXCEPT_EXPR(Expression) false
#else
#  define AUTODDS_NOEXCEPT noexcept
#  define AUTODDS_NOEXCEPT_OR_NOTHROW noexcept
#  define AUTODDS_NOEXCEPT_IF(Predicate) noexcept((Predicate))
#  define AUTODDS_NOEXCEPT_EXPR(Expression) noexcept((Expression))
#endif

#ifndef AUTODDS_FALLTHROUGH
#  define AUTODDS_FALLTHROUGH ((void)0)
#endif

#if defined(AUTODDS_NO_CXX11_CONSTEXPR)
#define AUTODDS_CONSTEXPR
#define AUTODDS_CONSTEXPR_OR_CONST const
#else
#define AUTODDS_CONSTEXPR constexpr
#define AUTODDS_CONSTEXPR_OR_CONST constexpr
#endif
#if defined(AUTODDS_NO_CXX14_CONSTEXPR)
#define AUTODDS_CXX14_CONSTEXPR
#else
#define AUTODDS_CXX14_CONSTEXPR constexpr
#endif
#if !defined(AUTODDS_NO_CXX17_STRUCTURED_BINDINGS) && defined(AUTODDS_NO_CXX11_HDR_TUPLE)
#  define AUTODDS_NO_CXX17_STRUCTURED_BINDINGS
#endif

// C++17 inline variables
//
#if !defined(AUTODDS_NO_CXX17_INLINE_VARIABLES)
#define AUTODDS_INLINE_VARIABLE inline
#else
#define AUTODDS_INLINE_VARIABLE
#endif

#if !defined(AUTODDS_NO_CXX17_IF_CONSTEXPR)
#  define AUTODDS_IF_CONSTEXPR if constexpr
#else
#  define AUTODDS_IF_CONSTEXPR if
#endif
#define AUTODDS_INLINE_CONSTEXPR  AUTODDS_INLINE_VARIABLE AUTODDS_CONSTEXPR_OR_CONST

#ifndef AUTODDS_ATTRIBUTE_UNUSED
#  if defined(__has_attribute) && defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x5130)
#    if __has_attribute(maybe_unused)
#       define AUTODDS_ATTRIBUTE_UNUSED [[maybe_unused]]
#    endif
#  elif defined(__has_cpp_attribute)
#    if __has_cpp_attribute(maybe_unused)
#      define AUTODDS_ATTRIBUTE_UNUSED [[maybe_unused]]
#    endif
#  endif
#endif

#ifndef AUTODDS_ATTRIBUTE_UNUSED
#  define AUTODDS_ATTRIBUTE_UNUSED
#endif

#if defined(__has_attribute) && defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x5130)
#if __has_attribute(nodiscard)
# define AUTODDS_ATTRIBUTE_NODISCARD [[nodiscard]]
#endif
#if __has_attribute(no_unique_address)
# define AUTODDS_ATTRIBUTE_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
#elif defined(__has_cpp_attribute)
// clang-6 accepts [[nodiscard]] with -std=c++14, but warns about it -pedantic
#if __has_cpp_attribute(nodiscard) && !(defined(__clang__) && (__cplusplus < 201703L)) && !(defined(__GNUC__) && (__cplusplus < 201100))
# define AUTODDS_ATTRIBUTE_NODISCARD [[nodiscard]]
#endif
#if __has_cpp_attribute(no_unique_address) && !(defined(__GNUC__) && (__cplusplus < 201100))
# define AUTODDS_ATTRIBUTE_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
#endif
#ifndef AUTODDS_ATTRIBUTE_NODISCARD
# define AUTODDS_ATTRIBUTE_NODISCARD
#endif
#ifndef AUTODDS_ATTRIBUTE_NO_UNIQUE_ADDRESS
# define AUTODDS_ATTRIBUTE_NO_UNIQUE_ADDRESS
#endif

#if !defined(AUTODDS_NO_CXX11_NULLPTR)
# define AUTODDS_NULLPTR nullptr
#else
# define AUTODDS_NULLPTR 0
#endif

#if !defined(AUTODDS_NO_CXX11_STATIC_ASSERT) && !defined(AUTODDS_HAS_STATIC_ASSERT)
#  define AUTODDS_HAS_STATIC_ASSERT
#endif

#if !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_HAS_RVALUE_REFS)
#define AUTODDS_HAS_RVALUE_REFS
#endif

#if !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES) && !defined(AUTODDS_HAS_VARIADIC_TMPL)
#define AUTODDS_HAS_VARIADIC_TMPL
#endif

#if defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES) && !defined(AUTODDS_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS)
#  define AUTODDS_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#endif

#if !defined(_YVALS) && !defined(_CPPLIB_VER)  // msvc std lib already configured
#if (!defined(__has_include) || (__cplusplus < 201700))
#  define AUTODDS_NO_CXX17_HDR_OPTIONAL
#  define AUTODDS_NO_CXX17_HDR_STRING_VIEW
#  define AUTODDS_NO_CXX17_HDR_VARIANT
#  define AUTODDS_NO_CXX17_HDR_ANY
#  define AUTODDS_NO_CXX17_HDR_MEMORY_RESOURCE
#  define AUTODDS_NO_CXX17_HDR_CHARCONV
#  define AUTODDS_NO_CXX17_HDR_EXECUTION
#  define AUTODDS_NO_CXX17_HDR_FILESYSTEM
#else
#if !__has_include(<optional>)
#  define AUTODDS_NO_CXX17_HDR_OPTIONAL
#endif
#if !__has_include(<string_view>)
#  define AUTODDS_NO_CXX17_HDR_STRING_VIEW
#endif
#if !__has_include(<variant>)
#  define AUTODDS_NO_CXX17_HDR_VARIANT
#endif
#if !__has_include(<any>)
#  define AUTODDS_NO_CXX17_HDR_ANY
#endif
#if !__has_include(<memory_resource>)
#  define AUTODDS_NO_CXX17_HDR_MEMORY_RESOURCE
#endif
#if !__has_include(<charconv>)
#  define AUTODDS_NO_CXX17_HDR_CHARCONV
#endif
#if !__has_include(<execution>)
#  define AUTODDS_NO_CXX17_HDR_EXECUTION
#endif
#if !__has_include(<filesystem>)
#  define AUTODDS_NO_CXX17_HDR_FILESYSTEM
#endif
#endif
#endif

#ifndef AUTODDS_CXX_VERSION
#  define AUTODDS_CXX_VERSION __cplusplus
#endif

#if (!defined(__has_include) || (AUTODDS_CXX_VERSION < 201704))
#  define AUTODDS_NO_CXX20_HDR_BARRIER
#  define AUTODDS_NO_CXX20_HDR_FORMAT
#  define AUTODDS_NO_CXX20_HDR_SOURCE_LOCATION
#  define AUTODDS_NO_CXX20_HDR_BIT
#  define AUTODDS_NO_CXX20_HDR_LATCH
#  define AUTODDS_NO_CXX20_HDR_SPAN
#  define AUTODDS_NO_CXX20_HDR_COMPARE
#  define AUTODDS_NO_CXX20_HDR_NUMBERS
#  define AUTODDS_NO_CXX20_HDR_STOP_TOKEN
#  define AUTODDS_NO_CXX20_HDR_CONCEPTS
#  define AUTODDS_NO_CXX20_HDR_RANGES
#  define AUTODDS_NO_CXX20_HDR_SYNCSTREAM
#  define AUTODDS_NO_CXX20_HDR_COROUTINE
#  define AUTODDS_NO_CXX20_HDR_SEMAPHORE
#else
#if (!__has_include(<barrier>) || !defined(__cpp_lib_barrier) || (__cpp_lib_barrier < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_BARRIER)
#  define AUTODDS_NO_CXX20_HDR_BARRIER
#endif
#if (!__has_include(<format>) || !defined(__cpp_lib_format) || (__cpp_lib_format < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_FORMAT)
#  define AUTODDS_NO_CXX20_HDR_FORMAT
#endif
#if (!__has_include(<source_location>) || !defined(__cpp_lib_source_location) || (__cpp_lib_source_location < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_SOURCE_LOCATION)
#  define AUTODDS_NO_CXX20_HDR_SOURCE_LOCATION
#endif
#if (!__has_include(<bit>) || !defined(__cpp_lib_bit_cast) || (__cpp_lib_bit_cast < 201806L) || !defined(__cpp_lib_bitops) || (__cpp_lib_bitops < 201907L) || !defined(__cpp_lib_endian) || (__cpp_lib_endian < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_BIT)
#  define AUTODDS_NO_CXX20_HDR_BIT
#endif
#if (!__has_include(<latch>) || !defined(__cpp_lib_latch) || (__cpp_lib_latch < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_LATCH)
#  define AUTODDS_NO_CXX20_HDR_LATCH
#endif
#if (!__has_include(<span>) || !defined(__cpp_lib_span) || (__cpp_lib_span < 202002L)) && !defined(AUTODDS_NO_CXX20_HDR_SPAN)
#  define AUTODDS_NO_CXX20_HDR_SPAN
#endif
#if (!__has_include(<compare>) || !defined(__cpp_lib_three_way_comparison) || (__cpp_lib_three_way_comparison < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_COMPARE)
#  define AUTODDS_NO_CXX20_HDR_COMPARE
#endif
#if (!__has_include(<numbers>) || !defined(__cpp_lib_math_constants) || (__cpp_lib_math_constants < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_NUMBERS)
#  define AUTODDS_NO_CXX20_HDR_NUMBERS
#endif
#if (!__has_include(<stop_token>) || !defined(__cpp_lib_jthread) || (__cpp_lib_jthread < 201911L)) && !defined(AUTODDS_NO_CXX20_HDR_STOP_TOKEN)
#  define AUTODDS_NO_CXX20_HDR_STOP_TOKEN
#endif
#if (!__has_include(<concepts>) || !defined(__cpp_lib_concepts) || (__cpp_lib_concepts < 202002L)) && !defined(_YVALS) && !defined(_CPPLIB_VER) && !defined(AUTODDS_NO_CXX20_HDR_CONCEPTS)
#  define AUTODDS_NO_CXX20_HDR_CONCEPTS
#endif
#if (!__has_include(<ranges>) || !defined(__cpp_lib_ranges) || (__cpp_lib_ranges < 201911L)) && !defined(AUTODDS_NO_CXX20_HDR_RANGES)
#  define AUTODDS_NO_CXX20_HDR_RANGES
#endif
#if (!__has_include(<syncstream>) || !defined(__cpp_lib_syncbuf) || (__cpp_lib_syncbuf < 201803L)) && !defined(AUTODDS_NO_CXX20_HDR_SYNCSTREAM)
#  define AUTODDS_NO_CXX20_HDR_SYNCSTREAM
#endif
#if (!__has_include(<coroutine>) || !defined(__cpp_lib_coroutine) || (__cpp_lib_coroutine < 201902L)) && !defined(AUTODDS_NO_CXX20_HDR_COROUTINE)
#  define AUTODDS_NO_CXX20_HDR_COROUTINE
#endif
#if (!__has_include(<semaphore>) || !defined(__cpp_lib_semaphore) || (__cpp_lib_semaphore < 201907L)) && !defined(AUTODDS_NO_CXX20_HDR_SEMAPHORE)
#  define AUTODDS_NO_CXX20_HDR_SEMAPHORE
#endif
#endif

#if (!defined(__has_include) || (AUTODDS_CXX_VERSION < 202003L))
#  define AUTODDS_NO_CXX23_HDR_EXPECTED
#  define AUTODDS_NO_CXX23_HDR_FLAT_MAP
#  define AUTODDS_NO_CXX23_HDR_FLAT_SET
#  define AUTODDS_NO_CXX23_HDR_GENERATOR
#  define AUTODDS_NO_CXX23_HDR_MDSPAN
#  define AUTODDS_NO_CXX23_HDR_PRINT
#  define AUTODDS_NO_CXX23_HDR_SPANSTREAM
#  define AUTODDS_NO_CXX23_HDR_STACKTRACE
#  define AUTODDS_NO_CXX23_HDR_STDFLOAT
#else
#if (!__has_include(<expected>) || !defined(__cpp_lib_expected) || (__cpp_lib_expected < 202211L)) && !defined(AUTODDS_NO_CXX23_HDR_EXPECTED)
#  define AUTODDS_NO_CXX23_HDR_EXPECTED
#endif
#if (!__has_include(<flat_map>) || !defined(__cpp_lib_flat_map) || (__cpp_lib_flat_map < 202207L)) && !defined(AUTODDS_NO_CXX23_HDR_FLAT_MAP)
#  define AUTODDS_NO_CXX23_HDR_FLAT_MAP
#endif
#if (!__has_include(<flat_set>) || !defined(__cpp_lib_flat_set) || (__cpp_lib_flat_set < 202207L)) && !defined(AUTODDS_NO_CXX23_HDR_FLAT_SET)
#  define AUTODDS_NO_CXX23_HDR_FLAT_SET
#endif
#if (!__has_include(<generator>) || !defined(__cpp_lib_generator) || (__cpp_lib_generator < 202207L)) && !defined(AUTODDS_NO_CXX23_HDR_GENERATOR)
#  define AUTODDS_NO_CXX23_HDR_GENERATOR
#endif
#if (!__has_include(<mdspan>) || !defined(__cpp_lib_mdspan) || (__cpp_lib_mdspan < 202207L)) && !defined(AUTODDS_NO_CXX23_HDR_MDSPAN)
#  define AUTODDS_NO_CXX23_HDR_MDSPAN
#endif
#if (!__has_include(<print>) || !defined(__cpp_lib_print) || (__cpp_lib_print < 202207L)) && !defined(AUTODDS_NO_CXX23_HDR_PRINT)
#  define AUTODDS_NO_CXX23_HDR_PRINT
#endif
#if (!__has_include(<spanstream>) || !defined(__cpp_lib_spanstream) || (__cpp_lib_spanstream < 202106L)) && !defined(AUTODDS_NO_CXX23_HDR_SPANSTREAM)
#  define AUTODDS_NO_CXX23_HDR_SPANSTREAM
#endif
#if (!__has_include(<stacktrace>) || !defined(__cpp_lib_stacktrace) || (__cpp_lib_stacktrace < 202011L)) && !defined(AUTODDS_NO_CXX23_HDR_STACKTRACE)
#  define AUTODDS_NO_CXX23_HDR_STACKTRACE
#endif
#if !__has_include(<stdfloat>) && !defined(AUTODDS_NO_CXX23_HDR_STDFLOAT)
#  define AUTODDS_NO_CXX23_HDR_STDFLOAT
#endif
#endif

#if defined(__cplusplus) && defined(__has_include)
#if !__has_include(<version>)
#  define AUTODDS_NO_CXX20_HDR_VERSION
#else
// For convenience, this is always included:
#  include <version>
#endif
#else
#  define AUTODDS_NO_CXX20_HDR_VERSION
#endif

#if defined(AUTODDS_MSVC)
#if (AUTODDS_MSVC < 1914) || (_MSVC_LANG < 201703)
#  define AUTODDS_NO_CXX17_DEDUCTION_GUIDES
#endif
#elif !defined(__cpp_deduction_guides) || (__cpp_deduction_guides < 201606)
#  define AUTODDS_NO_CXX17_DEDUCTION_GUIDES
#endif

#include "libs/config/detail/cxx_composite.hpp"

#if defined(AUTODDS_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(AUTODDS_CONFIG_ALLOW_DEPRECATED)
#  error "You are using a compiler which lacks features which are now a minimum requirement in order to use AUTODDS, define AUTODDS_CONFIG_ALLOW_DEPRECATED if you want to continue at your own risk!!!"
#endif


#endif //AUTODDS_LIBS_CONFIG_DETAIL_SUFFIX_HPP_