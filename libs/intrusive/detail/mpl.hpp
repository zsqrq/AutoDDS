//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/type_traits.hpp"

namespace autodds {
namespace intrusive {
namespace detail {

using autodds::libs::move_detail::is_same;
using autodds::libs::move_detail::add_const;
using autodds::libs::move_detail::remove_const;
using autodds::libs::move_detail::remove_cv;
using autodds::libs::move_detail::remove_reference;
using autodds::libs::move_detail::add_reference;
using autodds::libs::move_detail::remove_pointer;
using autodds::libs::move_detail::add_pointer;
using autodds::libs::move_detail::true_type;
using autodds::libs::move_detail::false_type;
using autodds::libs::move_detail::voider;
using autodds::libs::move_detail::enable_if_c;
using autodds::libs::move_detail::enable_if;
using autodds::libs::move_detail::disable_if_c;
using autodds::libs::move_detail::disable_if;
using autodds::libs::move_detail::is_convertible;
using autodds::libs::move_detail::if_c;
using autodds::libs::move_detail::if_;
using autodds::libs::move_detail::is_const;
using autodds::libs::move_detail::identity;
using autodds::libs::move_detail::alignment_of;
using autodds::libs::move_detail::is_empty;
using autodds::libs::move_detail::addressof;
using autodds::libs::move_detail::integral_constant;
using autodds::libs::move_detail::enable_if_convertible;
using autodds::libs::move_detail::disable_if_convertible;
using autodds::libs::move_detail::bool_;
using autodds::libs::move_detail::true_;
using autodds::libs::move_detail::false_;
using autodds::libs::move_detail::yes_type;
using autodds::libs::move_detail::no_type;
using autodds::libs::move_detail::apply;
using autodds::libs::move_detail::eval_if_c;
using autodds::libs::move_detail::eval_if;
using autodds::libs::move_detail::unvoid_ref;
using autodds::libs::move_detail::add_const_if_c;

template <std::size_t N>
struct ls_zeros
{
  static const std::size_t value = (N & std::size_t(1)) ? 0 : (1 + ls_zeros<(N >> 1u)>::value);
};

template<>
struct ls_zeros<0>
{
  static const std::size_t value = 0;
};


}
}
}

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_