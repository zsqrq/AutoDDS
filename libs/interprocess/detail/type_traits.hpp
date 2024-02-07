//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

struct nat{};

template<class T>
struct remove_reference
{ typedef T type; };

template<class T>
struct remove_reference<T&>
{ typedef T type; };

template<class T>
struct is_reference
{ static const bool value = false; };

template<class T>
struct is_reference<T&>
{ static const bool value = true; };

template<class T>
struct is_pointer
{ static const bool value = false;};

template<class T>
struct is_pointer<T*>
{ static const bool value = true; };

template<class T>
struct add_reference
{ typedef T& type; };

template<class T>
struct add_reference<T&>
{ typedef T& type; };

template<>
struct add_reference<void>
{ typedef nat& type; };

template<>
struct add_reference<const void>
{ typedef const nat& type; };

template<class T>
struct add_const_reference
{ typedef const T& type; };

template<class T>
struct add_const_reference<T&>
{ typedef T& type; };

template<class T>
struct remove_const
{ typedef T type;};

template<class T>
struct remove_const<const T>
{ typedef T type;};

template<class T>
struct remove_volatile
{ typedef T type; };

template<class T>
struct remove_volatile<volatile T>
{ typedef T type; };

template<class T>
struct remove_const_volatile
{ typedef typename remove_const<typename remove_volatile<T>::type>::type type; };

template<typename T, typename U>
struct is_same
{
  typedef char yes_type;
  struct no_type{
    char padding[8];
  };

  template<typename V>
  static yes_type test(V*,V*);
  static no_type test(...);

  static T* t;
  static U* u;

  static const bool value = (sizeof(yes_type) == sizeof(test(t,u)));
};

template<class T, class U>
struct is_cv_same
{
  static const bool value = is_same< typename remove_const_volatile<T>::type
      , typename remove_const_volatile<U>::type >::value;
};

}
}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP_