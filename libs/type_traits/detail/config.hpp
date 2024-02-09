//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_DETAIL_CONFIG_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_DETAIL_CONFIG_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif

#include "libs/config/workaround.hpp"

#define AUTODDS_TT_DECL /**/

# if (AUTODDS_WORKAROUND(__MWERKS__, < 0x3000)                         \
    || AUTODDS_WORKAROUND(__IBMCPP__, < 600 )                         \
    || AUTODDS_WORKAROUND(AUTODDS_BORLANDC, < 0x5A0)                      \
    || defined(__ghs)                                               \
    || AUTODDS_WORKAROUND(__HP_aCC, < 60700)           \
    || AUTODDS_WORKAROUND(MPW_CPLUS, AUTODDS_TESTED_AT(0x890))          \
    || AUTODDS_WORKAROUND(__SUNPRO_CC, AUTODDS_TESTED_AT(0x580)))       \
    && defined(AUTODDS_NO_IS_ABSTRACT)

#   define AUTODDS_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION 1
#endif

#ifndef AUTODDS_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION
# define AUTODDS_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION 1
#endif

#if defined(_MSC_EXTENSIONS) && !defined(AUTODDS_BORLANDC)
#  define AUTODDS_TT_TEST_MS_FUNC_SIGS
#endif

#if AUTODDS_WORKAROUND(__MWERKS__, < 0x3000) || AUTODDS_WORKAROUND(__IBMCPP__, <= 600)
#  define AUTODDS_TT_NO_CV_FUNC_TEST
#endif

#if !defined(AUTODDS_NO_SFINAE_EXPR) && !defined(AUTODDS_NO_CXX11_DECLTYPE) && !AUTODDS_WORKAROUND(AUTODDS_MSVC, < 1900) && !AUTODDS_WORKAROUND(AUTODDS_GCC, < 40900)
#  define AUTODDS_TT_HAS_ACCURATE_BINARY_OPERATOR_DETECTION
#endif

#if defined(__clang__) && (__clang_major__ == 3) && (__clang_minor__ < 2) && defined(AUTODDS_TT_HAS_ACCURATE_BINARY_OPERATOR_DETECTION)
#undef AUTODDS_TT_HAS_ACCURATE_BINARY_OPERATOR_DETECTION
#endif

#if !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES) && !AUTODDS_WORKAROUND(AUTODDS_GCC, < 40805)\
      && !AUTODDS_WORKAROUND(AUTODDS_MSVC, < 1900) && !AUTODDS_WORKAROUND(__clang_major__, <= 4)
#  define AUTODDS_TT_HAS_ACCURATE_IS_FUNCTION
#endif

#if defined(__cpp_rvalue_references) && !defined(__cpp_noexcept_function_type) && !defined(AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE)
#  define AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE
#endif

#if defined(AUTODDS_MSVC) && !defined(__cpp_rvalue_references) && !defined(AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE) && !defined(_NOEXCEPT_TYPES_SUPPORTED)
#  define AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE
#endif
#if defined(__cpp_rvalue_references) && defined(__NVCC__) && defined(__CUDACC__) && !defined(AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE)
#  define AUTODDS_TT_NO_NOEXCEPT_SEPARATE_TYPE
#endif

#endif //AUTODDS_LIBS_TYPE_TRAITS_DETAIL_CONFIG_HPP_