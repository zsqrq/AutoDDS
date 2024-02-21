//
// Created by wz on 24-2-20.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_DESTRUCTIBLE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_DESTRUCTIBLE_HPP_

#include "libs/type_traits/integral_constant.hpp"
#include "libs/config/workaround.hpp"
#include "libs/type_traits/is_complete.hpp"
#include "libs/common/static_assert.hpp"
#include "libs/type_traits/detail/yes_no_type.hpp"
#include "libs/type_traits/declval.hpp"

namespace autodds {
namespace libs {
namespace detail {

struct is_destructible_imp
{
  template <typename T, typename  = decltype(declval<T&>().~T())>
  static autodds::libs::type_traits::yes_tpye test(int);

  template<typename >
  static autodds::libs::type_traits::no_type test(...);
};

} // namespace detail

template <typename T>
struct is_destructible : public integral_constant<bool, sizeof(detail::is_destructible_imp::test<T>(0)) == sizeof(type_traits::yes_tpye)>
{
  AUTODDS_STATIC_ASSERT_MSG(autodds::libs::is_complete<T>::value,"Arguments to is_destructible must be complete types");
};

template <> struct is_destructible<void> : public false_type {};
template <> struct is_destructible<void const> : public false_type {};
template <> struct is_destructible<void volatile> : public false_type {};
template <> struct is_destructible<void const volatile> : public false_type {};
template <typename T> struct is_destructible<T&> : public is_destructible<T>{};
template <typename T> struct is_destructible<T&&> : public is_destructible<T> {};
template <typename T, std::size_t N>
struct is_destructible<T[N]> : public is_destructible<T> {};
template <typename T>
struct is_destructible<T[]> : public is_destructible<T> {};
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_DESTRUCTIBLE_HPP_