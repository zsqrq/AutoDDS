//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_ABSTRACT_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_ABSTRACT_HPP_

#include "libs/type_traits/intrinsics.hpp"
#include "libs/type_traits/integral_constant.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace detail {

#ifdef AUTODDS_IS_ABSTRACT

template <typename T>
struct is_abstract_imp
{
  AUTODDS_STATIC_CONSTANT(bool, value = AUTODDS_IS_ABSTRACT(T));
};

#endif

} // namespace detail

#ifndef AUTODDS_NO_IS_ABSTRACT
template <typename T>
struct is_abstract : public integral_constant<bool, detail::is_abstract_imp<T>::value> {};
#endif
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_ABSTRACT_HPP_