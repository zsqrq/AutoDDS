//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_POINTER_ELEMENT_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_POINTER_ELEMENT_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#ifndef AUTODDS_LIBS_MOVE_DETAIL_WORKAROUND_HPP_
#include "libs/move/detail/workaround.hpp"
#endif

namespace autodds {
namespace movelib {
namespace detail {

//struct first_param
template <typename T>
struct first_param
{ typedef void type; };

#if !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES)
template <template<typename , typename...> class TemplateClass, typename T, typename... Args>
struct first_param <TemplateClass <T, Args...>>
{
  typedef T type;
};
#endif

template <typename T>
struct has_internal_pointer_element
{
  template<typename X>
  static char test(int, typename X::element_type*);

  template<typename X>
  static int test(...);

  static const bool value = (1 == sizeof(test<T>(0,0)));
};

template <typename Ptr, bool = has_internal_pointer_element<Ptr>::value>
struct pointer_element_impl
{
  typedef typename Ptr::element_type type;
};

template <typename Ptr>
struct pointer_element_impl<Ptr, false>
{
  typedef typename first_param<Ptr>::type  type;
};

} // namespace detail

template <typename Ptr>
struct pointer_element
{
  typedef typename detail::pointer_element_impl<Ptr>::type type;
};

template <typename T>
struct pointer_element<T*>
{  typedef T type; };

} // namespace movelib
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_DETAIL_POINTER_ELEMENT_HPP_