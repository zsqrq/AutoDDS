//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP_

#include "libs/type_traits/add_reference.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct add_lvalue_reference
{
  typedef typename add_reference<T>::type type;
};

template <typename T>
struct add_lvalue_reference<T&&>
{
  typedef T& type;
};

template <class T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP_