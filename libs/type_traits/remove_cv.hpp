//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_REMOVE_CV_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_REMOVE_CV_HPP_

#include "libs/config/config.hpp"
#include "libs/config/workaround.hpp"
#include <cstddef>

namespace autodds {
namespace libs {

template <typename T>
struct remove_cv { typedef T type; };
template <typename T>
struct remove_cv<T const> { typedef T type; };
template <typename T>
struct remove_cv<T volatile> { typedef T type; };
template <typename T>
struct remove_cv<T const volatile> { typedef T type; };

template <typename T, std::size_t N>
struct remove_cv<T const[N]> { typedef T type[N]; };
template <typename T, std::size_t N>
struct remove_cv<T const volatile[N]> { typedef T type[N]; };
template <typename T, std::size_t N>
struct remove_cv<T volatile[N]> { typedef T type[N]; };

template <typename T>
struct remove_cv<T const[]> { typedef T type[]; };
template <typename T>
struct remove_cv<T const volatile[]> { typedef T type[];};
template <typename T>
struct remove_cv<T volatile[]> { typedef T type[];};

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_REMOVE_CV_HPP_