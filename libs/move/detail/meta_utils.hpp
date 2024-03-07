//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_META_UTILS_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_META_UTILS_HPP_

#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/addressof.hpp"
#include <cstddef>
#include <type_traits>

namespace autodds {
namespace libs {

template <class T> class rv;

namespace move_detail {

template<typename T>
struct voider { typedef void type; };

//             if_c
template<bool C, typename T1, typename T2>
struct if_c
{ typedef T1 type; };

template<typename T1, typename T2>
struct if_c<false,T1,T2>
{ typedef T2 type; };

//             if_
template<typename T1, typename T2, typename T3>
struct if_ : if_c<0 != T1::value, T2, T3>
{};

//          enable_if_c
struct enable_if_nat{};

template <bool B, class T = enable_if_nat>
struct enable_if_c
{ typedef T type; };

template <class T>
struct enable_if_c<false, T> {};

//   enable_if
template <class Cond, class T = enable_if_nat>
struct enable_if : enable_if_c<Cond::value, T> {};

//          disable_if
template <bool B, class T = enable_if_nat>
struct disable_if_c
    : enable_if_c<!B, T>
{};

template <class Cond, class T = enable_if_nat>
struct disable_if : enable_if_c<!Cond::value, T> {};

//          integral_constant
template<class T, T v>
struct integral_constant
{
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;

  operator T() const { return value; }
  T operator()() const { return value; }
};

typedef integral_constant<bool, true >  true_type;
typedef integral_constant<bool, false > false_type;

//             is_same
template<class T, class U>
struct is_same
{ static const bool value = false; };

template<class T>
struct is_same<T, T>
{ static const bool value = true; };

//        enable_if_same
template <class T, class U, class R = enable_if_nat>
struct enable_if_same : enable_if<is_same<T, U>, R> {};

//        disable_if_same
template <class T, class U, class R = enable_if_nat>
struct disable_if_same : disable_if<is_same<T, U>, R> {};

//          is_different
template<class T, class U>
struct is_different
{ static const bool value = !std::is_same<T, U>::value; };

template<class F, class Param>
struct apply 
{ typedef typename F::template apply<Param>::type type; };

//             bool_
template< bool C_ >
struct bool_ : integral_constant<bool, C_>
{
  AUTODDS_MOVE_FORCEINLINE operator bool() const { return C_; }
  AUTODDS_MOVE_FORCEINLINE bool operator()() const { return C_; }
};
typedef bool_<true>        true_;
typedef bool_<false>       false_;

//              nat
struct nat{};
struct nat2{};
struct nat3{};

template <unsigned N>
struct natN
{};

// yes_type/no_type
typedef char yes_type;

struct no_type {
  char _[2];
};

// natify
template <class T> struct natify{};

// remove_reference
template<class T>
struct remove_reference
{ typedef T type; };

template<class T>
struct remove_reference<T&>
{ typedef T type; };

template<class T>
struct remove_reference<T&&>
{ typedef T type; };

// remove_pointer
template< class T > struct remove_pointer                    { typedef T type;   };
template< class T > struct remove_pointer<T*>                { typedef T type;   };
template< class T > struct remove_pointer<T* const>          { typedef T type;   };
template< class T > struct remove_pointer<T* volatile>       { typedef T type;   };
template< class T > struct remove_pointer<T* const volatile> { typedef T type;   };

// add_pointer
template< class T >
struct add_pointer
{ typedef typename remove_reference<T>::type* type; };

//  add_const
template<class T>
struct add_const
{ typedef const T type; };

template<class T>
struct add_const<T&>
{ typedef const T& type; };

template<class T>
struct add_const<T&&>
{ typedef T&& type; };

// add_lvalue_reference
template<class T>
struct add_lvalue_reference
{  typedef T& type;  };

template<class T> struct add_lvalue_reference<T&>                 {  typedef T& type;  };
template<>        struct add_lvalue_reference<void>               {  typedef void type;   };
template<>        struct add_lvalue_reference<const void>         {  typedef const void type;  };
template<>        struct add_lvalue_reference<volatile void>      {  typedef volatile void type;   };
template<>        struct add_lvalue_reference<const volatile void>{  typedef const volatile void type;   };

template<class T>
struct add_const_lvalue_reference
{
  typedef typename remove_reference<T>::type         t_unreferenced;
  typedef typename add_const<t_unreferenced>::type   t_unreferenced_const;
  typedef typename add_lvalue_reference
      <t_unreferenced_const>::type                    type;
};

//  is_lvalue_reference
template<class T>
struct is_lvalue_reference
{ static const bool value = false; };

template<class T>
struct is_lvalue_reference<T&>
{ static const bool value = true; };

// identity
template <class T>
struct identity
{
  typedef T type;
  typedef typename add_const_lvalue_reference<T>::type reference;
  AUTODDS_MOVE_FORCEINLINE reference operator()(reference t) const
  {  return t;   }
};

// is_class_or_union
template<class T>
struct is_class_or_union
{
  struct twochar { char dummy[2]; };
  template <class U>
  static char is_class_or_union_tester(void(U::*)(void));
  template <class U>
  static twochar is_class_or_union_tester(...);
  static const bool value = sizeof(is_class_or_union_tester<T>(0)) == sizeof(char);
};

//          has_pointer_type
template <class T>
struct has_pointer_type
{
  struct two { char c[2]; };
  template <class U> static two test(...);
  template <class U> static char test(typename U::pointer* = 0);
  static const bool value = sizeof(test<T>(0)) == 1;
};

// is_convertible
template <class T, class U>
class is_convertible
{
  typedef typename add_lvalue_reference<T>::type t_reference;
  typedef char true_t;
  class false_t { char dummy[2]; };
  static false_t dispatch(...);
  static true_t  dispatch(U);
  static t_reference       trigger();
 public:
  static const bool value = sizeof(dispatch(trigger())) == sizeof(true_t);
};

template <class T, class U, bool IsSame = is_same<T, U>::value>
struct is_same_or_convertible : is_convertible<T, U> {};

template <class T, class U>
struct is_same_or_convertible<T, U, true>
{ static const bool value = true; };

template <bool C, typename F1, typename F2>
struct eval_if_c : if_c<C,F1,F2>::type {};

template <typename C, typename T1, typename T2>
struct eval_if : if_<C,T1,T2>::type {};

template<class T, class U, class R = void>
struct enable_if_convertible : enable_if< is_convertible<T, U>, R> {};

template<class T, class U, class R = void>
struct disable_if_convertible : disable_if< is_convertible<T, U>, R> {};

template<class T, class U, class R = void>
struct enable_if_same_or_convertible
    : enable_if< is_same_or_convertible<T, U>, R> {};

template<class T, class U, class R = void>
struct disable_if_same_or_convertible
    : disable_if< is_same_or_convertible<T, U>, R> {};

// and_
template<bool, class B = true_, class C = true_, class D = true_>
struct and_impl
    : and_impl<B::value, C, D>
{};

template<>
struct and_impl<true, true_, true_, true_>
{
  static const bool value = true;
};

template <class B, class C, class D>
struct and_impl<false, B, C, D>
{
  static const bool value = false;
};

template <class A, class B, class C = true_, class D = true_>
struct and_
    : and_impl<A::value, B, C, D>
{};

// or_
template <bool, class B = false_, class C = false_, class D = false_>
struct or_impl
    : or_impl<B::value, C, D>
{};

template<>
struct or_impl <false, false_, false_, false_>
{
  static const bool value = false;
};

template<class B, class C, class D>
struct or_impl <true, B, C, D>
{
  static const bool value = true;
};

template<class A, class B, class C = false_, class D = false_>
struct or_
    : or_impl<A::value, B, C, D>
{};

//  not_
template<class T>
struct not_
{
  static const bool value = !T::value;
};

// enable_if_and / disable_if_and / enable_if_or / disable_if_or
template<class R, class A, class B, class C = true_, class D = true_>
struct enable_if_and
    : enable_if_c< and_<A, B, C, D>::value, R>
{};

template<class R, class A, class B, class C = true_, class D = true_>
struct disable_if_and
    : disable_if_c< and_<A, B, C, D>::value, R>
{};

template<class R, class A, class B, class C = false_, class D = false_>
struct enable_if_or
    : enable_if_c< or_<A, B, C, D>::value, R>
{};

template<class R, class A, class B, class C = false_, class D = false_>
struct disable_if_or
    : disable_if_c< or_<A, B, C, D>::value, R>
{};

// has_move_emulation_enabled_impl
template<class T>
struct has_move_emulation_enabled_impl
    : is_convertible< T, ::autodds::libs::rv<T>& >
{};

template<class T>
struct has_move_emulation_enabled_impl<T&>
{  static const bool value = false;  };

template<class T>
struct has_move_emulation_enabled_impl< ::autodds::libs::rv<T> >
{  static const bool value = false;  };

//  is_rv_impl
template <class T>
struct is_rv_impl
{  static const bool value = false;  };

template <class T>
struct is_rv_impl< rv<T> >
{  static const bool value = true;  };

template <class T>
struct is_rv_impl< const rv<T> >
{  static const bool value = true;  };

template< class T >
struct is_rvalue_reference
{  static const bool value = false;  };

template< class T >
struct is_rvalue_reference< T&& >
{  static const bool value = true;  };

template< class T >
struct add_rvalue_reference
{ typedef T&& type; };

template< class T > struct remove_rvalue_reference { typedef T type; };
template< class T > struct remove_rvalue_reference< T&& > { typedef T type; };

}
}
}

#endif //AUTODDS_LIBS_MOVE_DETAIL_META_UTILS_HPP_