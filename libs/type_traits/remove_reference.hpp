//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_REMOVE_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_REMOVE_REFERENCE_HPP_

#include "libs/config/config.hpp"
#include "libs/config/workaround.hpp"

namespace autodds {
namespace libs {
namespace detail {

template <typename T>
struct remove_rvalue_ref
{ typedef T type; };

template <typename T>
struct remove_rvalue_ref<T&&>
{ typedef T type;};

}

template <typename T>
struct remove_reference
{
  typedef typename detail::remove_rvalue_ref<T>::type type;
};
template <typename T>
struct remove_reference<T&>
{ typedef T type;};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;
}
}


#endif //AUTODDS_LIBS_TYPE_TRAITS_REMOVE_REFERENCE_HPP_