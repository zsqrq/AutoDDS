//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP_

#include "libs/type_traits/integral_constant.hpp"
#include "libs/config/workaround.hpp"
#include "libs/type_traits/is_destructible.hpp"
#include "libs/type_traits/is_default_constructible.hpp"
#include "libs/type_traits/detail/yes_no_type.hpp"
#include "libs/type_traits/declval.hpp"
#include "libs/type_traits/is_complete.hpp"
#include "libs/common/static_assert.hpp"

#define AUTODDS_TT_IS_CONSTRUCTIBLE_CONFORMING 1

namespace autodds {
namespace libs {
namespace detail {

struct is_constructible_imp
{
  template<typename T, typename ...Args, typename = decltype(T(autodds::libs::declval<Args>()...))>
  static autodds::libs::type_traits::yes_tpye test(int);
  template<typename, typename ...>
  static autodds::libs::type_traits::no_type test(...);

  template<typename T, typename Arg, typename = decltype(new T(autodds::libs::declval<Arg>()))>
  static autodds::libs::type_traits::yes_tpye test1(int);
  template <typename , typename>
  static autodds::libs::type_traits::no_type test1(...);

  template<typename T>
  static autodds::libs::type_traits::yes_tpye ref_test(T);
  template<typename T>
  static autodds::libs::type_traits::no_type ref_test(...);
};

} // namespace detail

template <typename T, typename... Args>
struct is_constructible : public integral_constant<bool, sizeof(detail::is_constructible_imp::test<T,Args...>(0)) ==
    sizeof(autodds::libs::type_traits::yes_tpye)>
{
  AUTODDS_STATIC_ASSERT_MSG(autodds::libs::is_complete<T>::value, "The target type must be complete in order to test for constructibility");
};
template <typename T, typename Arg>
struct is_constructible<T, Arg> : public integral_constant<bool, is_destructible<T>::value &&
    sizeof(detail::is_constructible_imp::test1<T,Arg>(0)) == sizeof(autodds::libs::type_traits::yes_tpye)>
{
  AUTODDS_STATIC_ASSERT_MSG(autodds::libs::is_complete<T>::value, "The target type must be complete in order to test for constructibility");
};

template <typename Ref, typename Arg>
struct is_constructible<Ref&, Arg> : public integral_constant<bool,
    sizeof(detail::is_constructible_imp::ref_test<Ref&>(declval<Arg>()))== sizeof(type_traits::yes_tpye)> {};
template <typename Ref, typename Arg>
struct is_constructible<Ref&&, Arg> : public integral_constant<bool,
    sizeof(detail::is_constructible_imp::ref_test<Ref&&>(declval<Arg>())) == sizeof(type_traits::yes_tpye)> {};

template <> struct is_constructible<void> : public false_type{};
template <> struct is_constructible<void const> : public false_type{};
template <> struct is_constructible<void const volatile> : public false_type{};
template <> struct is_constructible<void volatile> : public false_type{};

template <typename T> struct is_constructible<T> : public is_default_constructible<T>{};

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP_