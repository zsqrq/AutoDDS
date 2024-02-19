//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_VOID_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_VOID_HPP_

#  include "libs/config/config.hpp"
#include "libs/type_traits/integral_constant.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_void : public false_type {};

template<>
struct is_void<void> : public true_type {};
template<>
struct is_void<const void> : public true_type {};
template<>
struct is_void<const volatile void> : public true_type {};
template<>
struct is_void<volatile void > : public true_type {};

}
}




#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_VOID_HPP_