//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_STDLIB_LIBSTDCPP3_HPP_
#define AUTODDS_LIBS_CONFIG_STDLIB_LIBSTDCPP3_HPP_

#define AUTODDS_GNU_STDLIB 1

#ifdef __GLIBCXX__
#define AUTODDS_STDLIB "GNU libstdc++ version " AUTODDS_STRINGIZE(__GLIBCXX__)
#else
#define AUTODDS_STDLIB "GNU libstdc++ version " AUTODDS_STRINGIZE(__GLIBCPP__)
#endif

#if !defined(_GLIBCPP_USE_WCHAR_T) && !defined(_GLIBCXX_USE_WCHAR_T)
#  define AUTODDS_NO_CWCHAR
#  define AUTODDS_NO_CWCTYPE
#  define AUTODDS_NO_STD_WSTRING
#  define AUTODDS_NO_STD_WSTREAMBUF
#endif

#if defined(__osf__) && !defined(_REENTRANT) \
  && ( defined(_GLIBCXX_HAVE_GTHR_DEFAULT) || defined(_GLIBCPP_HAVE_GTHR_DEFAULT) )
// GCC 3 on Tru64 forces the definition of _REENTRANT when any std lib header
// file is included, therefore for consistency we define it here as well.
#  define _REENTRANT
#endif

#ifdef __GLIBCXX__ // gcc 3.4 and greater:
#  if defined(_GLIBCXX_HAVE_GTHR_DEFAULT) \
        || defined(_GLIBCXX__PTHREADS) \
        || defined(_GLIBCXX_HAS_GTHREADS) \
        || defined(_WIN32) \
        || defined(_AIX) \
        || defined(__HAIKU__)
      //
      // If the std lib has thread support turned on, then turn it on in AUTODDS
      // as well.  We do this because some gcc-3.4 std lib headers define _REENTANT
      // while others do not...
      //
#     define AUTODDS_HAS_THREADS
#  else
#     define AUTODDS_DISABLE_THREADS
#  endif
#elif defined(__GLIBCPP__) \
        && !defined(_GLIBCPP_HAVE_GTHR_DEFAULT) \
        && !defined(_GLIBCPP__PTHREADS)
// disable thread support if the std lib was built single threaded:
#  define AUTODDS_DISABLE_THREADS
#endif

#if (defined(linux) || defined(__linux) || defined(__linux__)) && defined(__arm__) && defined(_GLIBCPP_HAVE_GTHR_DEFAULT)
// linux on arm apparently doesn't define _REENTRANT
// so just turn on threading support whenever the std lib is thread safe:
#  define AUTODDS_HAS_THREADS
#endif

#if !defined(_GLIBCPP_USE_LONG_LONG) \
    && !defined(_GLIBCXX_USE_LONG_LONG)\
    && defined(AUTODDS_HAS_LONG_LONG)
// May have been set by compiler/*.hpp, but "long long" without library
// support is useless.
#  undef AUTODDS_HAS_LONG_LONG
#endif

#if !defined(CYGWIN) && defined(__unix__) || defined(__unix)  || defined(unix)
#  include <unistd.h>
#endif

#if defined(__has_include)
#if defined(AUTODDS_HAS_HASH)
#if !__has_include(AUTODDS_HASH_SET_HEADER) || (__GNUC__ >= 10)
#undef AUTODDS_HAS_HASH
#undef AUTODDS_HAS_SET_HEADER
#undef AUTODDS_HAS_MAP_HEADER
#endif
#if !__has_include(AUTODDS_SLIST_HEADER)
#undef AUTODDS_HAS_SLIST
#undef AUTODDS_HAS_SLIST_HEADER
#endif
#endif
#endif
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103)
#  define AUTODDS_LIBSTDCXX11
#endif

#ifdef __clang__

#ifdef _GLIBCXX_RELEASE
#  define AUTODDS_LIBSTDCXX_VERSION (_GLIBCXX_RELEASE * 10000 + 100)
#else
//
// We figure out which gcc version issued this std lib
// by checking which headers are available:
//
#if __has_include(<expected>)
#  define AUTODDS_LIBSTDCXX_VERSION 120100
#elif __has_include(<source_location>)
#  define AUTODDS_LIBSTDCXX_VERSION 110100
#elif __has_include(<compare>)
#  define AUTODDS_LIBSTDCXX_VERSION 100100
#elif __has_include(<memory_resource>)
#  define AUTODDS_LIBSTDCXX_VERSION 90100
#elif __has_include(<charconv>)
#  define AUTODDS_LIBSTDCXX_VERSION 80100
#elif __has_include(<variant>)
#  define AUTODDS_LIBSTDCXX_VERSION 70100
#elif __has_include(<experimental/memory_resource>)
#  define AUTODDS_LIBSTDCXX_VERSION 60100
#elif __has_include(<experimental/any>)
#  define AUTODDS_LIBSTDCXX_VERSION 50100
#elif __has_include(<shared_mutex>)
#  define AUTODDS_LIBSTDCXX_VERSION 40900
#elif __has_include(<ext/cmath>)
#  define AUTODDS_LIBSTDCXX_VERSION 40800
#elif __has_include(<scoped_allocator>)
#  define AUTODDS_LIBSTDCXX_VERSION 40700
#elif __has_include(<typeindex>)
#  define AUTODDS_LIBSTDCXX_VERSION 40600
#elif __has_include(<future>)
#  define AUTODDS_LIBSTDCXX_VERSION 40500
#elif  __has_include(<ratio>)
#  define AUTODDS_LIBSTDCXX_VERSION 40400
#elif __has_include(<array>)
#  define AUTODDS_LIBSTDCXX_VERSION 40300
#endif
#endif
//
// If AUTODDS_HAS_FLOAT128 is set, now that we know the std lib is libstdc++3, check to see if the std lib is
// configured to support this type.  If not disable it:
//
#if defined(AUTODDS_HAS_FLOAT128) && !defined(_GLIBCXX_USE_FLOAT128)
#  undef AUTODDS_HAS_FLOAT128
#endif

#if (AUTODDS_LIBSTDCXX_VERSION >= 100000) && defined(AUTODDS_HAS_HASH)
//
// hash_set/hash_map deprecated and have terminal bugs:
//
#undef AUTODDS_HAS_HASH
#undef AUTODDS_HAS_SET_HEADER
#undef AUTODDS_HAS_MAP_HEADER
#endif


#if (AUTODDS_LIBSTDCXX_VERSION >= 100000) && defined(AUTODDS_HAS_HASH)
//
// hash_set/hash_map deprecated and have terminal bugs:
//
#undef AUTODDS_HAS_HASH
#undef AUTODDS_HAS_SET_HEADER
#undef AUTODDS_HAS_MAP_HEADER
#endif


#if (AUTODDS_LIBSTDCXX_VERSION < 50100)
// libstdc++ does not define this function as it's deprecated in C++11, but clang still looks for it,
// defining it here is a terrible cludge, but should get things working:
extern "C" char *gets (char *__s);
#endif
//
// clang is unable to parse some GCC headers, add those workarounds here:
//
#if AUTODDS_LIBSTDCXX_VERSION < 50000
#  define AUTODDS_NO_CXX11_HDR_REGEX
#endif
//
// GCC 4.7.x has no __cxa_thread_atexit which
// thread_local objects require for cleanup:
//
#if AUTODDS_LIBSTDCXX_VERSION < 40800
#  define AUTODDS_NO_CXX11_THREAD_LOCAL
#endif
//
// Early clang versions can handle <chrono>, not exactly sure which versions
// but certainly up to clang-3.8 and gcc-4.6:
//
#if (__clang_major__ < 5)
#  if AUTODDS_LIBSTDCXX_VERSION < 40800
#     define AUTODDS_NO_CXX11_HDR_FUTURE
#     define AUTODDS_NO_CXX11_HDR_MUTEX
#     define AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE
#     define AUTODDS_NO_CXX11_HDR_CHRONO
#  endif
#endif

//
//  GCC 4.8 and 9 add working versions of <atomic> and <regex> respectively.
//  However, we have no test for these as the headers were present but broken
//  in early GCC versions.
//
#endif

#if defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130) && (__cplusplus >= 201103L)
//
// Oracle Solaris compiler uses it's own verison of libstdc++ but doesn't
// set __GNUC__
//
#if __SUNPRO_CC >= 0x5140
#define AUTODDS_LIBSTDCXX_VERSION 50100
#else
#define AUTODDS_LIBSTDCXX_VERSION 40800
#endif
#endif

#if !defined(AUTODDS_LIBSTDCXX_VERSION)
#  define AUTODDS_LIBSTDCXX_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#if defined(AUTODDS_LIBSTDCXX11)
#  if AUTODDS_LIBSTDCXX_VERSION < 40600
#     if !_GLIBCXX_DEPRECATED
#        define AUTODDS_NO_AUTO_PTR
#     endif
#  elif !defined(_GLIBCXX_USE_DEPRECATED) || !_GLIBCXX_USE_DEPRECATED
#     define AUTODDS_NO_AUTO_PTR
#     define AUTODDS_NO_CXX98_BINDERS
#  endif
#endif

//
//  C++17 features in GCC 7.1 and later
//
#if (AUTODDS_LIBSTDCXX_VERSION < 70100) || (__cplusplus <= 201402L)
#  define AUTODDS_NO_CXX17_STD_INVOKE
#  define AUTODDS_NO_CXX17_STD_APPLY
#  define AUTODDS_NO_CXX17_HDR_OPTIONAL
#  define AUTODDS_NO_CXX17_HDR_STRING_VIEW
#  define AUTODDS_NO_CXX17_HDR_VARIANT
#endif

#if defined(__has_include)
#if !__has_include(<shared_mutex>)
#  define AUTODDS_NO_CXX14_HDR_SHARED_MUTEX
#elif __cplusplus <= 201103
#  define AUTODDS_NO_CXX14_HDR_SHARED_MUTEX
#endif

#if (AUTODDS_LIBSTDCXX_VERSION < 100100)
#if !__has_include(<tbb/tbb.h>)
#define AUTODDS_NO_CXX17_HDR_EXECUTION
#endif
#endif
#elif __cplusplus < 201402 || (AUTODDS_LIBSTDCXX_VERSION < 40900) || !defined(AUTODDS_LIBSTDCXX11)
#  define AUTODDS_NO_CXX14_HDR_SHARED_MUTEX
#endif

#if AUTODDS_LIBSTDCXX_VERSION < 100100
//
// The header may be present but is incomplete:
//
#  define AUTODDS_NO_CXX17_HDR_CHARCONV
#endif

#if AUTODDS_LIBSTDCXX_VERSION < 110000
//
// Header <bit> may be present but lacks std::bit_cast:
//
#define AUTODDS_NO_CXX20_HDR_BIT
#endif

#if AUTODDS_LIBSTDCXX_VERSION >= 120000
//
// Unary function is now deprecated in C++11 and later:
//
#if __cplusplus >= 201103L
#define AUTODDS_NO_CXX98_FUNCTION_BASE
#endif
#endif

#ifndef __cpp_impl_coroutine
#  define AUTODDS_NO_CXX20_HDR_COROUTINE
#endif

//
#if !defined(__cpp_lib_concepts)
#if !defined(AUTODDS_NO_CXX20_HDR_COMPARE)
#  define AUTODDS_NO_CXX20_HDR_COMPARE
#endif
#if !defined(AUTODDS_NO_CXX20_HDR_CONCEPTS)
#  define AUTODDS_NO_CXX20_HDR_CONCEPTS
#endif
#if !defined(AUTODDS_NO_CXX20_HDR_SPAN)
#  define AUTODDS_NO_CXX20_HDR_SPAN
#endif
#if !defined(AUTODDS_NO_CXX20_HDR_RANGES)
#  define AUTODDS_NO_CXX20_HDR_RANGES
#endif
#endif

#if defined(__clang__)
#if (__clang_major__ < 11) && !defined(AUTODDS_NO_CXX20_HDR_RANGES)
#  define AUTODDS_NO_CXX20_HDR_RANGES
#endif
#if (__clang_major__ < 10) && (AUTODDS_LIBSTDCXX_VERSION >= 110000) && !defined(AUTODDS_NO_CXX11_HDR_CHRONO)
// Old clang can't parse <chrono>:
#  define AUTODDS_NO_CXX11_HDR_CHRONO
#  define AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE
#endif
#endif


#if (!defined(_GLIBCXX_HAS_GTHREADS) || !defined(_GLIBCXX_USE_C99_STDINT_TR1))
// Headers not always available:
#  ifndef AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE
#     define AUTODDS_NO_CXX11_HDR_CONDITION_VARIABLE
#  endif
#  ifndef AUTODDS_NO_CXX11_HDR_MUTEX
#     define AUTODDS_NO_CXX11_HDR_MUTEX
#  endif
#  ifndef AUTODDS_NO_CXX11_HDR_THREAD
#     define AUTODDS_NO_CXX11_HDR_THREAD
#  endif
#  ifndef AUTODDS_NO_CXX14_HDR_SHARED_MUTEX
#     define AUTODDS_NO_CXX14_HDR_SHARED_MUTEX
#  endif
#endif

#if (!defined(_GTHREAD_USE_MUTEX_TIMEDLOCK) || (_GTHREAD_USE_MUTEX_TIMEDLOCK == 0)) && !defined(AUTODDS_NO_CXX11_HDR_MUTEX) && (__GNUC__ < 6)
// Timed mutexes are not always available:
#  define BOOST_NO_CXX11_HDR_MUTEX
#endif


#endif //AUTODDS_LIBS_CONFIG_STDLIB_LIBSTDCPP3_HPP_