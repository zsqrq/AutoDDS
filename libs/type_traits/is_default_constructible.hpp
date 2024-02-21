//
// Created by wz on 24-2-21.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP_

#include <cstddef>
#include "libs/type_traits/integral_constant.hpp"
#include "libs/config/workaround.hpp"
#include "libs/type_traits/is_complete.hpp"

#include "libs/common/static_assert.hpp"
#include "libs/type_traits/detail/yes_no_type.hpp"

#if AUTODDS_WORKAROUND(AUTODDS_GCC_VERSION, < 40700)
#include "libs/type_traits/is_abstract.hpp"
#endif

#if defined(__GNUC__) && (__GNUC__ <= 5)
#include <utility> // std::pair
#endif

namespace autodds {
namespace libs {
namespace detail {

struct is_default_constructible_imp
{
  template<typename Tp, typename  = decltype(Tp())>
  static type_traits::yes_tpye test(int);

  template<typename >
  static type_traits::no_type test(...);
};

} // namespace detail

template <typename T>
struct is_default_constructible : public integral_constant<bool,
    sizeof(detail::is_default_constructible_imp::test<T>(0)) == sizeof(type_traits::yes_tpye)>
{
  AUTODDS_STATIC_ASSERT_MSG(autodds::libs::is_complete<T>::value, "Arguments to is_default_constructible must be complete types");
};

template <typename T, std::size_t N>
struct is_default_constructible<T[N]> : public is_default_constructible<T> {};
template <typename T>
struct is_default_constructible<T[]> : public is_default_constructible<T> {};
template <typename T>
struct is_default_constructible<T&> : public integral_constant<bool, false> {};
template <typename T>
struct is_default_constructible<T&&> : public integral_constant<bool, false> {};
template <> struct is_default_constructible<void> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void const> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void volatile> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void const volatile> : public integral_constant<bool, false>{};

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP_