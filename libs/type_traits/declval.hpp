//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_DECLVAL_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_DECLVAL_HPP_

#include "libs/type_traits/add_rvalue_reference.hpp"
#include "libs/config/config.hpp"

namespace autodds {
namespace libs {

template <typename T>
typename type_traits_detail::add_rvalue_reference<T>::type declval() AUTODDS_NOEXCEPT;

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_DECLVAL_HPP_