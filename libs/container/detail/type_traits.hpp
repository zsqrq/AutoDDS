//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_TYPE_TRAITS_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_TYPE_TRAITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif 
#
#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

using ::autodds::libs::move_detail::enable_if;
using ::autodds::libs::move_detail::enable_if_and;
using ::autodds::libs::move_detail::is_same;
using ::autodds::libs::move_detail::is_different;
using ::autodds::libs::move_detail::is_pointer;
using ::autodds::libs::move_detail::add_reference;
using ::autodds::libs::move_detail::add_const;
using ::autodds::libs::move_detail::add_const_reference;
using ::autodds::libs::move_detail::remove_const;
using ::autodds::libs::move_detail::remove_reference;
using ::autodds::libs::move_detail::remove_cvref;
using ::autodds::libs::move_detail::make_unsigned;
using ::autodds::libs::move_detail::is_floating_point;
using ::autodds::libs::move_detail::is_integral;
using ::autodds::libs::move_detail::is_enum;
using ::autodds::libs::move_detail::is_pod;
using ::autodds::libs::move_detail::is_empty;
using ::autodds::libs::move_detail::is_trivially_destructible;
using ::autodds::libs::move_detail::is_trivially_default_constructible;
using ::autodds::libs::move_detail::is_trivially_copy_constructible;
using ::autodds::libs::move_detail::is_trivially_move_constructible;
using ::autodds::libs::move_detail::is_trivially_copy_assignable;
using ::autodds::libs::move_detail::is_trivially_move_assignable;
using ::autodds::libs::move_detail::is_nothrow_default_constructible;
using ::autodds::libs::move_detail::is_nothrow_copy_constructible;
using ::autodds::libs::move_detail::is_nothrow_move_constructible;
using ::autodds::libs::move_detail::is_nothrow_copy_assignable;
using ::autodds::libs::move_detail::is_nothrow_move_assignable;
using ::autodds::libs::move_detail::is_nothrow_swappable;
using ::autodds::libs::move_detail::alignment_of;
using ::autodds::libs::move_detail::aligned_storage;
using ::autodds::libs::move_detail::nat;
using ::autodds::libs::move_detail::nat2;
using ::autodds::libs::move_detail::nat3;
using ::autodds::libs::move_detail::natN;
using ::autodds::libs::move_detail::max_align_t;
using ::autodds::libs::move_detail::is_convertible;


} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_TYPE_TRAITS_HPP_