//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_INTRINSICS_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_INTRINSICS_HPP_

#ifndef AUTODDS_TT_DISABLE_INTRINSICS
#include "libs/config/config.hpp"

#ifndef  AUTODDS_LIBS_TYPE_TRAITS_DETAIL_CONFIG_HPP_
#  include "libs/type_traits/detail/config.hpp"
#endif

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) && !defined(__GCCXML__))) && !defined(AUTODDS_CLANG)
//
// Note that even though these intrinsics rely on other type traits classes
// we do not #include those here as it produces cyclic dependencies and
// can cause the intrinsics to not even be used at all!
//

#ifdef AUTODDS_INTEL
#  define AUTODDS_INTEL_TT_OPTS || is_pod<T>::value
#else
#  define AUTODDS_INTEL_TT_OPTS
#endif

#   define AUTODDS_IS_UNION(T) __is_union(T)
#   define AUTODDS_IS_POD(T) __is_pod(T)
#   define AUTODDS_IS_EMPTY(T) __is_empty(T)
#   define AUTODDS_HAS_TRIVIAL_CONSTRUCTOR(T) ((__has_trivial_constructor(T) AUTODDS_INTEL_TT_OPTS) && ! ::boost::is_volatile<T>::value)
#   define AUTODDS_HAS_TRIVIAL_COPY(T) ((__has_trivial_copy(T) AUTODDS_INTEL_TT_OPTS) && !is_reference<T>::value)
#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 409
#   define AUTODDS_HAS_TRIVIAL_ASSIGN(T) ((__has_trivial_assign(T) AUTODDS_INTEL_TT_OPTS) && ! ::boost::is_volatile<T>::value && ! ::boost::is_const<T>::value && is_assignable<T&, const T&>::value)
#   define AUTODDS_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) AUTODDS_INTEL_TT_OPTS && is_destructible<T>::value)
#   define AUTODDS_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) && is_default_constructible<T>::value AUTODDS_INTEL_TT_OPTS)
#   define AUTODDS_HAS_NOTHROW_COPY(T) ((__has_nothrow_copy(T) AUTODDS_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_reference<T>::value && is_copy_constructible<T>::value)
#   define AUTODDS_HAS_NOTHROW_ASSIGN(T) ((__has_nothrow_assign(T) AUTODDS_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_const<T>::value && is_assignable<T&, const T&>::value)
#else
#   define AUTODDS_HAS_TRIVIAL_ASSIGN(T) ((__has_trivial_assign(T) AUTODDS_INTEL_TT_OPTS) && ! ::boost::is_volatile<T>::value && ! ::boost::is_const<T>::value)
#   define AUTODDS_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) AUTODDS_INTEL_TT_OPTS)
#   define AUTODDS_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) AUTODDS_INTEL_TT_OPTS)
#if ((__GNUC__ * 100 + __GNUC_MINOR__) != 407) && ((__GNUC__ * 100 + __GNUC_MINOR__) != 408)
#   define AUTODDS_HAS_NOTHROW_COPY(T) ((__has_nothrow_copy(T) AUTODDS_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_reference<T>::value && !is_array<T>::value)
#endif
#   define AUTODDS_HAS_NOTHROW_ASSIGN(T) ((__has_nothrow_assign(T) AUTODDS_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_const<T>::value && !is_array<T>::value)
#endif
#   define AUTODDS_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define AUTODDS_IS_ABSTRACT(T) __is_abstract(T)
#   define AUTODDS_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_same<T,U>::value)
#   define AUTODDS_IS_CLASS(T) __is_class(T)
#   define AUTODDS_IS_ENUM(T) __is_enum(T)
#   define AUTODDS_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   if (!defined(unix) && !defined(__unix__) && \
       !(defined(__VXWORKS__) && defined(__i386__)))  || defined(__LP64__)
// GCC sometimes lies about alignment requirements
// of type double on 32-bit unix platforms, use the
// old implementation instead in that case:
#     define AUTODDS_ALIGNMENT_OF(T) __alignof__(T)
#   endif
#   if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
#     define AUTODDS_IS_FINAL(T) __is_final(T)
#   endif

#   if (__GNUC__ >= 5) && (__cplusplus >= 201103)
#     define AUTODDS_HAS_TRIVIAL_MOVE_ASSIGN(T) (__is_trivially_assignable(T&, T&&) && is_assignable<T&, T&&>::value && !::boost::is_volatile<T>::value)
#     define AUTODDS_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) (__is_trivially_constructible(T, T&&) && is_constructible<T, T&&>::value && !::boost::is_volatile<T>::value)
#   endif

#   define AUTODDS_HAS_TYPE_TRAITS_INTRINSICS
#endif


#endif
#endif //AUTODDS_LIBS_TYPE_TRAITS_INTRINSICS_HPP_