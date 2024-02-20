//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_ARITHMETIC_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_ARITHMETIC_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/type_traits/is_integral.hpp"
#include "libs/type_traits/is_floating_point.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_arithmetic : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_ARITHMETIC_HPP_