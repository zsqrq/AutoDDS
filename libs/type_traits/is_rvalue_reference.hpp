//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP_

#include "libs/type_traits/integral_constant.hpp"
#  include "libs/config/config.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_rvalue_reference : public false_type {};
template <typename T>
struct is_rvalue_reference<T&&> : public true_type{};

}
}


#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP_