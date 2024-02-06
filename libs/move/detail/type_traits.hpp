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

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/meta_utils.hpp"
#include "libs/move/detail/addressof.hpp"
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

#   define AUTODDS_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) AUTODDS_MOVE_INTEL_TT_OPTS)
#   define AUTODDS_MOVE_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) AUTODDS_MOVE_INTEL_TT_OPTS)
#   define AUTODDS_MOVE_HAS_NOTHROW_COPY(T) ((__has_nothrow_copy(T) AUTODDS_MOVE_INTEL_TT_OPTS))
#   define AUTODDS_MOVE_HAS_NOTHROW_ASSIGN(T) ((__has_nothrow_assign(T) AUTODDS_MOVE_INTEL_TT_OPTS))

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
#define AUTODDS_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T)    AUTODDS_MOVE_HAS_TRIVIAL_DESTRUCTOR(T) || std::is_pod<T>::value
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

template <class T>
struct add_const_reference<T&>
{  typedef T& type;   };

//    add_const_if_c
template<class T, bool Add>
struct add_const_if_c
    : if_c<Add, typename add_const<T>::type, T>
{};

//    remove_const
template<class T>
struct remove_const
{  typedef T type;   };

template<class T>
struct remove_const< const T>
{  typedef T type;   };

//    remove_cv
template<typename T> struct remove_cv                    {  typedef T type;   };
template<typename T> struct remove_cv<const T>           {  typedef T type;   };
template<typename T> struct remove_cv<const volatile T>  {  typedef T type;   };
template<typename T> struct remove_cv<volatile T>        {  typedef T type;   };

//    remove_cvref
template<class T>
struct remove_cvref
    : remove_cv<typename remove_reference<T>::type>
{};

//    make_unsigned
template <class T>
struct make_unsigned_impl                                         {  typedef T type;   };
template <> struct make_unsigned_impl<signed char>                {  typedef unsigned char  type; };
template <> struct make_unsigned_impl<signed short>               {  typedef unsigned short type; };
template <> struct make_unsigned_impl<signed int>                 {  typedef unsigned int   type; };
template <> struct make_unsigned_impl<signed long>                {  typedef unsigned long  type; };
#ifdef AUTODDS_HAS_LONG_LONG
template <> struct make_unsigned_impl< ::autodds::long_long_type >  {  typedef ::autodds::ulong_long_type type; };
#endif

template <class T>
struct make_unsigned
    : make_unsigned_impl<typename remove_cv<T>::type>
{};

//    is_floating_point
template<class T> struct is_floating_point_cv               {  static const bool value = false; };
template<>        struct is_floating_point_cv<float>        {  static const bool value = true; };
template<>        struct is_floating_point_cv<double>       {  static const bool value = true; };
template<>        struct is_floating_point_cv<long double>  {  static const bool value = true; };

template<class T>
struct is_floating_point
    : is_floating_point_cv<typename remove_cv<T>::type>
{};

//    is_integral
template<class T> struct is_integral_cv                     {  static const bool value = false; };
template<> struct is_integral_cv< bool >                    {  static const bool value = true; };
template<> struct is_integral_cv< char >                    {  static const bool value = true; };
template<> struct is_integral_cv< unsigned char >           {  static const bool value = true; };
template<> struct is_integral_cv< signed char >             {  static const bool value = true; };
#ifndef AUTODDS_NO_CXX11_CHAR16_T
template<> struct is_integral_cv< char16_t >                {  static const bool value = true; };
#endif
#ifndef AUTODDS_NO_CXX11_CHAR32_T
template<> struct is_integral_cv< char32_t >                {  static const bool value = true; };
#endif
#ifndef AUTODDS_NO_INTRINSIC_WCHAR_T
template<> struct is_integral_cv< wchar_t >                 {  static const bool value = true; };
#endif
template<> struct is_integral_cv< short >                   {  static const bool value = true; };
template<> struct is_integral_cv< unsigned short >          {  static const bool value = true; };
template<> struct is_integral_cv< int >                     {  static const bool value = true; };
template<> struct is_integral_cv< unsigned int >            {  static const bool value = true; };
template<> struct is_integral_cv< long >                    {  static const bool value = true; };
template<> struct is_integral_cv< unsigned long >           {  static const bool value = true; };
#ifdef AUTODDS_HAS_LONG_LONG
template<> struct is_integral_cv< ::autodds::long_long_type >{  static const bool value = true; };
template<> struct is_integral_cv< ::autodds::ulong_long_type>{  static const bool value = true; };
#endif
template<class T>
struct is_integral
    : public is_integral_cv<typename remove_cv<T>::type>
{};

// remove_all_extents
template <class T>
struct remove_all_extents
{  typedef T type;};

template <class T>
struct remove_all_extents<T[]>
{  typedef typename remove_all_extents<T>::type type; };

template <class T, std::size_t N>
struct remove_all_extents<T[N]>
{  typedef typename remove_all_extents<T>::type type;};

//    is_scalar
template<class T>
struct is_scalar
{  static const bool value = is_integral<T>::value || is_floating_point<T>::value; };

// is_void
template<class T>
struct is_void_cv
{  static const bool value = false; };

template<>
struct is_void_cv<void>
{  static const bool value = true; };

template<class T>
struct is_void
    : is_void_cv<typename remove_cv<T>::type>
{};

//  is_array
template<class T>
struct is_array
{  static const bool value = false; };

template<class T>
struct is_array<T[]>
{  static const bool value = true;  };

template<class T, std::size_t N>
struct is_array<T[N]>
{  static const bool value = true;  };

//           is_member_pointer
template <class T>
struct is_member_pointer_cv
{  static const bool value = false; };

template <class T, class U>
struct is_member_pointer_cv<T U::*>
{  static const bool value = true; };

template <class T>
struct is_member_pointer
    : is_member_pointer_cv<typename remove_cv<T>::type>
{};

// is_nullptr_t
template <class T>
struct is_nullptr_t_cv
{  static const bool value = false; };

#if !defined(AUTODDS_NO_CXX11_NULLPTR)
template <>
struct is_nullptr_t_cv
#if !defined(AUTODDS_NO_CXX11_DECLTYPE)
    <decltype(nullptr)>
#else
  <std::nullptr_t>
#endif
{  static const bool value = true; };
#endif

template <class T>
struct is_nullptr_t
    : is_nullptr_t_cv<typename remove_cv<T>::type>
{};

//          is_function
template <class T>
struct is_reference_convertible_to_pointer
{
  struct twochar { char dummy[2]; };
  template <class U> static char    test(U*);
  template <class U> static twochar test(...);
  static T& source();
  static const bool value = sizeof(char) == sizeof(test<T>(source()));
};

template < class T
    , bool Filter = is_class_or_union<T>::value  ||
        is_void<T>::value            ||
        is_reference<T>::value       ||
        is_nullptr_t<T>::value       >
struct is_function_impl
{  static const bool value = is_reference_convertible_to_pointer<T>::value; };

template <class T>
struct is_function_impl<T, true>
{  static const bool value = false; };

template <class T>
struct is_function
    : is_function_impl<T>
{};

//       is_union
template<class T>
struct is_union_noextents_cv
{  static const bool value = AUTODDS_MOVE_IS_UNION_IMPL(T); };

template<class T>
struct is_union
    : is_union_noextents_cv<typename remove_cv<typename remove_all_extents<T>::type>::type>
{};

//  is_class
template <class T>
struct is_class
{
  static const bool value = is_class_or_union<T>::value && ! is_union<T>::value;
};

//             is_arithmetic
template <class T>
struct is_arithmetic
{
  static const bool value = is_floating_point<T>::value ||
      is_integral<T>::value;
};

//    is_member_function_pointer
template <class T>
struct is_member_function_pointer_cv
{
  static const bool value = false;
};

template <class T, class C>
struct is_member_function_pointer_cv<T C::*>
    : is_function<T>
{};

template <class T>
struct is_member_function_pointer
    : is_member_function_pointer_cv<typename remove_cv<T>::type>
{};

//             is_enum
#if !defined(AUTODDS_MOVE_IS_ENUM)
//Based on (http://howardhinnant.github.io/TypeHiearchy.pdf)
template <class T>
struct is_enum_nonintrinsic
{
  static const bool value =  !is_arithmetic<T>::value     &&
      !is_reference<T>::value      &&
      !is_class_or_union<T>::value &&
      !is_array<T>::value          &&
      !is_void<T>::value           &&
      !is_nullptr_t<T>::value      &&
      !is_member_pointer<T>::value &&
      !is_pointer<T>::value        &&
      !is_function<T>::value;
};
#endif
template <class T>
struct is_enum
{  static const bool value = AUTODDS_MOVE_IS_ENUM_IMPL(T);  };

//       is_pod
template<class T>
struct is_pod_noextents_cv  //for non-c++11 compilers, a safe fallback
{  static const bool value = AUTODDS_MOVE_IS_POD_IMPL(T); };

template<class T>
struct is_pod
    : is_pod_noextents_cv<typename remove_cv<typename remove_all_extents<T>::type>::type>
{};

//             is_empty
template <class T>
struct is_empty
{  static const bool value = AUTODDS_MOVE_IS_EMPTY_IMPL(T);  };


template<class T>
struct has_autodds_move_no_copy_constructor_or_assign_type
{
  template <class U>
  static yes_type test(typename U::autodds_move_no_copy_constructor_or_assign*);

  template <class U>
  static no_type test(...);

  static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};

//       is_copy_constructible
template<class T>
struct is_copy_constructible
{
  template<class U> static typename add_reference<U>::type source();
  static no_type test(...);
  template <class U>
  static yes_type test(U&, decltype(U(source<U>()))* = 0);
  static const bool value = sizeof(test(source<T>())) == sizeof(yes_type);
};

//       is_copy_assignable
# define  AUTODDS_MOVE_TT_CXX11_IS_COPY_ASSIGNABLE
template <class T>
struct is_copy_assignable
{
#if defined(AUTODDS_MOVE_TT_CXX11_IS_COPY_ASSIGNABLE)
  typedef char yes_type;
   struct no_type { char dummy[2]; };
   
   template <class U>   static typename add_reference<U>::type source();
   template <class U>   static decltype(source<U&>() = source<const U&>(), yes_type() ) test(int);
   template <class>     static no_type test(...);

   static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
#else
  static const bool value = !has_autodds_move_no_copy_constructor_or_assign_type<T>::value;
#endif
};

//       is_trivially_destructible
template<class T>
struct is_trivially_destructible
{  static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_DESTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_default_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_default_constructible
{  static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_copy_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_copy_constructible
{
  static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T);
};

//////////////////////////////////////
//       is_trivially_move_constructible
//////////////////////////////////////
template<class T>
struct is_trivially_move_constructible
{ static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T); };

//////////////////////////////////////
//       is_trivially_copy_assignable
//////////////////////////////////////
template<class T>
struct is_trivially_copy_assignable
{
  static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_COPY_ASSIGNABLE(T);
};

//////////////////////////////////////
//       is_trivially_move_assignable
//////////////////////////////////////
template<class T>
struct is_trivially_move_assignable
{  static const bool value = AUTODDS_MOVE_IS_TRIVIALLY_MOVE_ASSIGNABLE(T);  };

//////////////////////////////////////
//       is_nothrow_default_constructible
//////////////////////////////////////
template<class T>
struct is_nothrow_default_constructible
{  static const bool value = AUTODDS_MOVE_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T);  };

//////////////////////////////////////
//    is_nothrow_copy_constructible
//////////////////////////////////////
template<class T>
struct is_nothrow_copy_constructible
{  static const bool value = AUTODDS_MOVE_IS_NOTHROW_COPY_CONSTRUCTIBLE(T);  };

//    is_nothrow_move_constructible
template<class T>
struct is_nothrow_move_constructible
{  static const bool value = AUTODDS_MOVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T);  };

//////////////////////////////////////
//       is_nothrow_copy_assignable
//////////////////////////////////////
template<class T>
struct is_nothrow_copy_assignable
{  static const bool value = AUTODDS_MOVE_IS_NOTHROW_COPY_ASSIGNABLE(T);  };

//////////////////////////////////////
//    is_nothrow_move_assignable
//////////////////////////////////////
template<class T>
struct is_nothrow_move_assignable
{  static const bool value = AUTODDS_MOVE_IS_NOTHROW_MOVE_ASSIGNABLE(T);  };

//////////////////////////////////////
//    is_nothrow_swappable
//////////////////////////////////////
template<class T>
struct is_nothrow_swappable
{
  static const bool value = is_empty<T>::value || is_pod<T>::value;
};

//       alignment_of
template <typename T>
struct alignment_of_hack
{
  T t1;
  char c;
  T t2;
  alignment_of_hack();
  ~alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{  static const std::size_t value = A < S ? A : S; };

template< typename T >
struct alignment_of_impl
{  static const std::size_t value = AUTODDS_MOVE_ALIGNMENT_OF(T);  };

template< typename T >
struct alignment_of
    : alignment_of_impl<T>
{};
class alignment_dummy;
typedef void (*function_ptr)();
typedef int (alignment_dummy::*member_ptr);

struct alignment_struct
{  long double dummy[4];  };

//    max_align_t
union max_align
{
  char        char_;
  short       short_;
  int         int_;
  long        long_;
#ifdef AUTODDS_HAS_LONG_LONG
  ::autodds::long_long_type   long_long_;
#endif
  float       float_;
  double      double_;
  void *      void_ptr_;
  long double long_double_[4];
  alignment_dummy *unknown_class_ptr_;
  function_ptr function_ptr_;
  alignment_struct alignment_struct_;
};
typedef union max_align max_align_t;

//    aligned_storage
template<std::size_t Len, std::size_t Align>
struct aligned_struct;
#define AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(A)\
template<std::size_t Len>\
struct AUTODDS_ALIGNMENT(A) aligned_struct<Len, A>\
{\
   unsigned char data[Len];\
};\
//
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x1)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x2)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x4)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x8)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x10)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x20)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x40)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x80)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x100)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x200)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x400)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x800)
AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT(0x1000)
#undef AUTODDS_MOVE_ALIGNED_STORAGE_WITH_AUTODDS_ALIGNMENT

template<std::size_t Len, std::size_t Align>
union aligned_struct_wrapper
{
  typedef aligned_struct<Len, Align> aligner_t;
  aligned_struct<Len, Align> aligner;
  unsigned char data[Len > sizeof(aligner_t) ? Len : sizeof(aligner_t)];
};

template<std::size_t Len, std::size_t Align>
struct aligned_storage_impl
{
  typedef aligned_struct_wrapper<Len, Align> type;
};

template<std::size_t Len, std::size_t Align = alignment_of<max_align_t>::value>
struct aligned_storage
{
  //Sanity checks for input parameters
  AUTODDS_MOVE_STATIC_ASSERT(Align > 0);

  //Sanity checks for output type
  typedef typename aligned_storage_impl<Len ? Len : 1, Align>::type type;
  static const std::size_t value = alignment_of<type>::value;
  AUTODDS_MOVE_STATIC_ASSERT(value >= Align);
  AUTODDS_MOVE_STATIC_ASSERT((value % Align) == 0);

  //Just in case someone instantiates aligned_storage
  //instead of aligned_storage::type (typical error).
 private:
  aligned_storage();
};

}
}
}

#endif //AUTODDS_LIBS_MOVE_DETAIL_TYPE_TRAITS_HPP_