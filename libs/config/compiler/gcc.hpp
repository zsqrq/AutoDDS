//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_COMPILER_GCC_HPP_
#define AUTODDS_LIBS_CONFIG_COMPILER_GCC_HPP_

#define AUTODDS_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if !defined(__CUDACC__)
#define AUTODDS_GCC AUTODDS_GCC_VERSION
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#  define AUTODDS_GCC_CXX11
#endif

// GCC prior to 3.4 had #pragma once too but it didn't work well with filesystem links
#if AUTODDS_GCC_VERSION >= 30400
#define AUTODDS_HAS_PRAGMA_ONCE
#endif

#if !defined(__EXCEPTIONS) && !defined(AUTODDS_NO_EXCEPTIONS)
# define AUTODDS_NO_EXCEPTIONS
#endif
//
// Threading support: Turn this on unconditionally here (except for
// those platforms where we can know for sure). It will get turned off again
// later if no threading API is detected.
//
#if !defined(__MINGW32__) && !defined(linux) && !defined(__linux) && !defined(__linux__)
# define AUTODDS_HAS_THREADS
#endif
// gcc has "long long"
// Except on Darwin with standard compliance enabled (-pedantic)
// Apple gcc helpfully defines this macro we can query
//
#if !defined(__DARWIN_NO_LONG_LONG)
# define AUTODDS_HAS_LONG_LONG
#endif

// Branch prediction hints
#define AUTODDS_LIKELY(x) __builtin_expect(x, 1)
#define AUTODDS_UNLIKELY(x) __builtin_expect(x, 0)

#if __GNUC__ >= 4

#define AUTODDS_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#define AUTODDS_SYMBOL_IMPORT
#define AUTODDS_SYMBOL_VISIBLE __attribute__((__visibility__("default")))
#else
#  define AUTODDS_SYMBOL_EXPORT
#endif

//
// RTTI and typeinfo detection is possible post gcc-4.3:
//
#if AUTODDS_GCC_VERSION > 40300
#  ifndef __GXX_RTTI
#     ifndef AUTODDS_NO_TYPEID
#        define AUTODDS_NO_TYPEID
#     endif
#     ifndef AUTODDS_NO_RTTI
#        define AUTODDS_NO_RTTI
#     endif
#  endif
#endif

#if defined(__CUDACC__)
#  if defined(AUTODDS_GCC_CXX11)
#    define AUTODDS_NVCC_CXX11
#  else
#    define AUTODDS_NVCC_CXX03
#  endif
#endif
#if defined(__SIZEOF_INT128__) && !defined(AUTODDS_NVCC_CXX03)
#  define AUTODDS_HAS_INT128
#endif
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif
#if defined(_GLIBCXX_USE_FLOAT128) && !defined(__STRICT_ANSI__) && !defined(AUTODDS_NVCC_CXX03)
# define AUTODDS_HAS_FLOAT128
#endif

// C++0x features in 4.3.n and later
//
#if (AUTODDS_GCC_VERSION >= 40300) && defined(AUTODDS_GCC_CXX11)
// C++0x features are only enabled when -std=c++0x or -std=gnu++0x are
// passed on the command line, which in turn defines
// __GXX_EXPERIMENTAL_CXX0X__.
#  define AUTODDS_HAS_DECLTYPE
#  define AUTODDS_HAS_RVALUE_REFS
#  define AUTODDS_HAS_STATIC_ASSERT
#  define AUTODDS_HAS_VARIADIC_TMPL
#else
#  define AUTODDS_NO_CXX11_DECLTYPE
#  define AUTODDS_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define AUTODDS_NO_CXX11_RVALUE_REFERENCES
#  define AUTODDS_NO_CXX11_STATIC_ASSERT
#endif

// C++ 14:
#if !defined(__cpp_aggregate_nsdmi) || (__cpp_aggregate_nsdmi < 201304)
#  define AUTODDS_NO_CXX14_AGGREGATE_NSDMI
#endif
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#  define AUTODDS_NO_CXX14_CONSTEXPR
#endif
#if (AUTODDS_GCC_VERSION < 50200) || !defined(__cpp_variable_templates) || (__cpp_variable_templates < 201304)
#  define AUTODDS_NO_CXX14_VARIABLE_TEMPLATES
#endif

// C++17
#if !defined(__cpp_structured_bindings) || (__cpp_structured_bindings < 201606)
#  define AUTODDS_NO_CXX17_STRUCTURED_BINDINGS
#endif
#if !defined(__cpp_inline_variables) || (__cpp_inline_variables < 201606)
#  define AUTODDS_NO_CXX17_INLINE_VARIABLES
#endif
#if !defined(__cpp_fold_expressions) || (__cpp_fold_expressions < 201603)
#  define AUTODDS_NO_CXX17_FOLD_EXPRESSIONS
#endif
#if !defined(__cpp_if_constexpr) || (__cpp_if_constexpr < 201606)
#  define AUTODDS_NO_CXX17_IF_CONSTEXPR
#endif

#if __GNUC__ >= 7
#  define AUTODDS_FALLTHROUGH __attribute__((fallthrough))
#endif

// Unused attribute:
#if __GNUC__ >= 4
#  define AUTODDS_ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

// Type aliasing hint. Supported since gcc 3.3.
#define AUTODDS_MAY_ALIAS __attribute__((__may_alias__))

// Unreachable code markup
#if AUTODDS_GCC_VERSION >= 40500
#define AUTODDS_UNREACHABLE_RETURN(x) __builtin_unreachable();
#endif

// Deprecated symbol markup
#if AUTODDS_GCC_VERSION >= 40500
#define AUTODDS_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define AUTODDS_DEPRECATED(msg) __attribute__((deprecated))
#endif

#ifndef AUTODDS_COMPILER
#  define AUTODDS_COMPILER "GNU C++ version " __VERSION__
#endif

// ConceptGCC compiler:
//   http://www.generic-programming.org/software/ConceptGCC/
#ifdef __GXX_CONCEPTS__
#  define AUTODDS_HAS_CONCEPTS
#  define AUTODDS_COMPILER "ConceptGCC version " __VERSION__
#endif

#if (AUTODDS_GCC_VERSION < 30300)
#  error "Compiler not configured - please reconfigure"
#endif

#if (AUTODDS_GCC_VERSION > 80100)
#  if defined(AUTODDS_ASSERT_CONFIG)
#     error "AUTODDS.Config is older than your compiler - please check for an updated AUTODDS release."
#  else
// we don't emit warnings here anymore since there are no defect macros defined for
// gcc post 3.4, so any failures are gcc regressions...
//#     warning "AUTODDS: Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif


#endif //AUTODDS_LIBS_CONFIG_COMPILER_GCC_HPP_