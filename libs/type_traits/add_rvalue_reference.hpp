//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP_

#  include "libs/config/config.hpp"
#include "libs/type_traits/is_void.hpp"
#include "libs/type_traits/is_reference.hpp"

namespace autodds {
namespace libs {
namespace type_traits_detail {

template <typename T, bool b>
struct add_rvalue_reference_helper
{ typedef T type; };

template <typename T>
struct add_rvalue_reference_helper<T, true>
{
  typedef T&& type;
};

template <typename T>
struct add_rvalue_reference_imp
{
  typedef typename add_rvalue_reference_helper<T,
  (is_void<T>::value == false && is_reference<T>::value == false)>::type type;
};

template <typename T>
struct add_rvalue_reference
{
  typedef typename add_rvalue_reference_imp<T>::type type;
};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

}
}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP_