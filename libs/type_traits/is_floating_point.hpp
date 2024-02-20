//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_FLOATING_POINT_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_FLOATING_POINT_HPP_

#include "libs/type_traits/is_integral.hpp"
#include "libs/type_traits/integral_constant.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_floating_point : public false_type {};

template <typename T>
struct is_floating_point<const T> : public is_floating_point<T> {};

template <typename T>
struct is_floating_point<volatile const T> : public is_floating_point<T> {};

template<> struct is_floating_point<float> : public true_type{};
template<> struct is_floating_point<double> : public true_type{};
template<> struct is_floating_point<long double> : public true_type{};

#if defined(AUTODDS_HAS_FLOAT128)
template<> struct is_floating_point<__float128> : public true_type{};
#endif



} // namespace autodds
} // namespace libs

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_FLOATING_POINT_HPP_