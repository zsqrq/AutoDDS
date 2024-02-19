//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_REFERENCE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_REFERENCE_HPP_

#include "libs/type_traits/is_lvalue_reference.hpp"
#include "libs/type_traits/is_rvalue_reference.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_reference : public integral_constant<bool,
    is_lvalue_reference<T>::value || is_rvalue_reference<T>::value>
{};

}
}


#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_REFERENCE_HPP_