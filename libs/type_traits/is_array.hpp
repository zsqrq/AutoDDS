//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_ARRAY_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_ARRAY_HPP_
#include "libs/config/config.hpp"
#include "libs/type_traits/integral_constant.hpp"
#include <cstddef>

namespace autodds {
namespace libs {

template <typename T>
struct is_array : public false_type {};
template <typename T, std::size_t N>
struct is_array<T[N]> : public true_type {};
template <typename T, std::size_t N>
struct is_array<T const[N]> : public true_type{};
template <typename T, std::size_t N>
struct is_array<T volatile[N]> : public true_type{};
template <typename T, std::size_t N>
struct is_array<T const volatile[N]> : public true_type{};

template <class T> struct is_array<T[]> : public true_type{};
template <class T> struct is_array<T const[]> : public true_type{};
template <class T> struct is_array<T const volatile[]> : public true_type{};
template <class T> struct is_array<T volatile[]> : public true_type{};

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_ARRAY_HPP_