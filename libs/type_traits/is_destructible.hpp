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
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_DESTRUCTIBLE_HPP_