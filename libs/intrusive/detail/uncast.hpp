//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_UNCAST_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_UNCAST_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/mpl.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<typename ConstNodePtr>
struct uncast_types
{
  typedef typename pointer_traits<ConstNodePtr>::element_type element_type;
  typedef typename autodds::libs::move_detail::remove_const<element_type>::type non_const_type;
  typedef typename pointer_traits<ConstNodePtr>::
  template rebind_pointer<non_const_type>::type               non_const_pointer;
  typedef pointer_traits<non_const_pointer>                   non_const_traits;
};

template<typename ConstNodePtr>
static typename uncast_types<ConstNodePtr>::non_const_pointer
uncast(const ConstNodePtr & ptr)
{
  return uncast_types<ConstNodePtr>::non_const_traits::const_cast_from(ptr);
}

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_UNCAST_HPP_