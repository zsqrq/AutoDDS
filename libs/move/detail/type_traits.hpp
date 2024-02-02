//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_TYPE_TRAITS_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_TYPE_TRAITS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif

#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <type_traits>

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) && !defined(__GCCXML__))) && !defined(AUTODDS_CLANG)
#   define AUTODDS_MOVE_INTEL_TT_OPTS
#   define AUTODDS_MOVE_IS_UNION(T) __is_union(T)
#   define AUTODDS_MOVE_IS_POD(T) __is_pod(T)
#   define AUTODDS_MOVE_IS_EMPTY(T) __is_empty(T)
#   define AUTODDS_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) ((__has_trivial_constructor(T) AUTODDS_MOVE_INTEL_TT_OPTS))

#   if defined(AUTODDS_GCC) && (AUTODDS_GCC > 50000)
#     define AUTODDS_MOVE_HAS_TRIVIAL_COPY(T) (__is_trivially_constructible(T, const T &))
#     define AUTODDS_MOVE_HAS_TRIVIAL_ASSIGN(T) (__is_trivially_assignable(T, const T &))
#   else
#     define AUTODDS_MOVE_HAS_TRIVIAL_COPY(T) ((__has_trivial_copy(T) AUTODDS_MOVE_INTEL_TT_OPTS))
#     define AUTODDS_MOVE_HAS_TRIVIAL_ASSIGN(T) ((__has_trivial_assign(T) AUTODDS_MOVE_INTEL_TT_OPTS) )
#   endif
#if !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_NO_CXX11_SFINAE_EXPR)

template <typename T>
T && autodds_move_tt_declval() AUTODDS_NOEXCEPT;
#  if defined(AUTODDS_GCC) && (AUTODDS_GCC >= 80000)
// __is_assignable / __is_constructible implemented
#     define AUTODDS_MOVE_IS_ASSIGNABLE(T, U)     __is_assignable(T, U)
#     define AUTODDS_MOVE_IS_CONSTRUCTIBLE(T, U)  __is_constructible(T, U)
#  else
template<typename Tt, typename Ut>
   class autodds_move_tt_is_assignable
   {
      struct twochar {  char dummy[2]; };
      template < class T
               , class U
               , class = decltype(autodds_move_tt_declval<T>() = autodds_move_tt_declval<U>())
               > static char test(int);

      template<class, class> static twochar test(...);

      public:
      static const bool value = sizeof(test<Tt, Ut>(0)) == sizeof(char);
   };

   template<typename Tt, typename Ut>
   class autodds_move_tt_is_constructible
   {
      struct twochar {  char dummy[2]; };
      template < class T
               , class U
               , class = decltype(T(autodds_move_tt_declval<U>()))
               > static char test(int);

      template<class, class> static twochar test(...);

      public:
      static const bool value = sizeof(test<Tt, Ut>(0)) == sizeof(char);
   };

#     define AUTODDS_MOVE_IS_ASSIGNABLE(T, U)     autodds_move_tt_is_assignable<T,U>::value
#     define AUTODDS_MOVE_IS_CONSTRUCTIBLE(T, U)  autodds_move_tt_is_constructible<T, U>::value
#endif
   template<typename T, typename U, bool = AUTODDS_MOVE_IS_ASSIGNABLE(T,U)>
   struct autodds_move_tt_is_nothow_assignable {
     static const bool value = false;
   };
   
   template <typename T, typename U>
   struct autodds_move_tt_is_nothow_assignable<T, U, true> {
     #if !defined(AUTODDS_NO_CXX11_NOEXCEPT)
     static const bool value = noexcept(autodds_move_tt_declval<T>() = autodds_move_tt_declval<U>());
     #else
     static const bool value = false;
     #endif
   };
   
   template <typename T, typename U, bool = AUTODDS_MOVE_IS_CONSTRUCTIBLE(T,U)>
   struct autodds_move_tt_is_nothrow_constructible {
     static const bool value = false;
   };
   
   template <typename T, typename U>
   struct autodds_move_tt_is_nothrow_constructible<T, U, true> {
     #if !defined(AUTODDS_NO_CXX11_NOEXCEPT)
     static const bool value = noexcept(T(autodds_move_tt_declval<U>()));
     #else
     static const bool value = false;
     #endif
   };
#     define AUTODDS_MOVE_HAS_NOTHROW_MOVE_ASSIGN(T)       autodds_move_tt_is_nothow_assignable<T, T&&>::value
#     define AUTODDS_MOVE_HAS_NOTHROW_MOVE_CONSTRUCTOR(T)  autodds_move_tt_is_nothrow_constructible<T, T&&>::value

#endif
#   define AUTODDS_MOVE_IS_ENUM(T) __is_enum(T)

#   if (!defined(unix) && !defined(__unix__)) || defined(__LP64__)
// GCC sometimes lies about alignment requirements
// of type double on 32-bit unix platforms, use the
// old implementation instead in that case:
#     define AUTODDS_MOVE_ALIGNMENT_OF(T) __alignof__(T)
#   endif
#endif

#ifdef AUTODDS_MOVE_IS_UNION
#define AUTODDS_MOVE_IS_UNION_IMPL(T) AUTODDS_MOVE_IS_UNION(T)
#else
#define AUTODDS_MOVE_IS_UNION_IMPL(T) false
#endif

#ifdef AUTODDS_MOVE_IS_POD
//in some compilers the intrinsic is limited to class types so add scalar and void
#define AUTODDS_MOVE_IS_POD_IMPL(T) (std::is_scalar<T>::value ||\
                                        std::is_void<T>::value   ||\
                                        AUTODDS_MOVE_IS_POD(T))
#else
#define AUTODDS_MOVE_IS_POD_IMPL(T) \
      (std::is_scalar<T>::value || std::is_void<T>::value)
#endif

#ifdef AUTODDS_MOVE_IS_EMPTY
#define AUTODDS_MOVE_IS_EMPTY_IMPL(T) AUTODDS_MOVE_IS_EMPTY(T)
#else
#define AUTODDS_MOVE_IS_EMPTY_IMPL(T)  0
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_COPY
#define AUTODDS_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   std::is_pod<T>::value ||\
                                                          (std::is_copy_constructible<T>::value &&\
                                                           AUTODDS_MOVE_HAS_TRIVIAL_COPY(T))
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)   std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_CONSTRUCTOR
#define AUTODDS_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)  AUTODDS_MOVE_HAS_TRIVIAL_CONSTRUCTOR(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)  std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR
#define AUTODDS_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)   AUTODDS_MOVE_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)   std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_ASSIGN
#define AUTODDS_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T)  std::is_pod<T>::value ||\
                                                      ( std::is_copy_assignable<T>::value &&\
                                                         AUTODDS_MOVE_HAS_TRIVIAL_ASSIGN(T))
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T) std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_MOVE_ASSIGN
#define AUTODDS_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)  AUTODDS_MOVE_HAS_TRIVIAL_MOVE_ASSIGN(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)  std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_TRIVIAL_DESTRUCTOR
#define AUTODDS_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T)   AUTODDS_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T)   std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_NOTHROW_CONSTRUCTOR
#define AUTODDS_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)  AUTODDS_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)  std::is_pod<T>::value
#endif

#ifdef AUTODDS_MOVE_HAS_NOTHROW_COPY
#define AUTODDS_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)   AUTODDS_MOVE_HAS_NOTHROW_COPY(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T)   AUTODDS_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T)
#endif

#ifdef AUTODDS_MOVE_HAS_NOTHROW_ASSIGN
#define AUTODDS_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T) AUTODDS_MOVE_HAS_NOTHROW_ASSIGN(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T) AUTODDS_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T)
#endif

#ifdef AUTODDS_MOVE_HAS_NOTHROW_MOVE_CONSTRUCTOR
#define AUTODDS_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)   AUTODDS_MOVE_HAS_NOTHROW_MOVE_CONSTRUCTOR(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)   AUTODDS_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)
#endif

#ifdef AUTODDS_MOVE_HAS_NOTHROW_MOVE_ASSIGN
#define AUTODDS_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T) AUTODDS_MOVE_HAS_NOTHROW_MOVE_ASSIGN(T) || std::is_pod<T>::value
#else
#define AUTODDS_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T) AUTODDS_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T)
#endif

#ifdef AUTODDS_MOVE_IS_ENUM
#define AUTODDS_MOVE_IS_ENUM_IMPL(T)   AUTODDS_MOVE_IS_ENUM(T)
#else
#define AUTODDS_MOVE_IS_ENUM_IMPL(T)   0
#endif

namespace autodds {
namespace libs {
namespace move_detail {

//    is_reference
template <typename T>
struct is_reference
{ static const bool value = false; };

template <typename T>
struct is_reference<T&>
{ static const bool value = true; };

#if !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES)
template<class T>
struct is_reference<T&&>
{  static const bool value = true; };
#endif

//    is_pointer
template <typename T>
struct is_pointer
{ static const bool value = false; };

template <typename T>
struct is_pointer<T*>
{ static const bool value = true; };

//       is_const
template <typename T>
struct is_const
{ static const bool value = false; };

template <typename T>
struct is_const<const T>
{ static const bool value = true; };



//       unvoid_ref
template <typename T> struct unvoid_ref : std::add_lvalue_reference<T>{};
template <> struct unvoid_ref<void>                { typedef unvoid_ref & type; };
template <> struct unvoid_ref<const void>          { typedef unvoid_ref & type; };
template <> struct unvoid_ref<volatile void>       { typedef unvoid_ref & type; };
template <> struct unvoid_ref<const volatile void> { typedef unvoid_ref & type; };

template <typename T>
struct add_reference : std::add_lvalue_reference<T>
{};

//    add_const_reference
template <typename T>
struct add_const_reference
{ typedef const T& type; };


}
}
}

#endif //AUTODDS_LIBS_MOVE_DETAIL_TYPE_TRAITS_HPP_