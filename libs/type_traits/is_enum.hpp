//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_ENUM_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_ENUM_HPP_

#include "libs/type_traits/intrinsics.hpp"
#include "libs/type_traits/integral_constant.hpp"
#include "libs/type_traits/detail/is_function_cxx_11.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_enum : public integral_constant<bool, AUTODDS_IS_ENUM(T)> {};

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_ENUM_HPP_