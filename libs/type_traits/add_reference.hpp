//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_ADD_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_ADD_REFERENCE_HPP_

#include "libs/config/workaround.hpp"
#include "libs/config/config.hpp"

namespace autodds {
namespace libs {
namespace detail {

template <typename T>
struct add_reference_impl
{ typedef T&& type; };

template <typename T>
struct add_reference_impl<T&&>
{
  typedef T&& type;
};

} // namespace detail

template <typename T>
struct add_reference
{
  typedef typename detail::add_reference_impl<T>::type type;
};

template <typename T>
struct add_reference<T&>
{
  typedef T& type;
};

template <> struct add_reference<void> { typedef void type; };
template <> struct add_reference<const void> { typedef const void type; };
template <> struct add_reference<const volatile void> { typedef const volatile void type; };
template <> struct add_reference<volatile void> { typedef volatile void type; };

template <class T> using add_reference_t = typename add_reference<T>::type;

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_ADD_REFERENCE_HPP_