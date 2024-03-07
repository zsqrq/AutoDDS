//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_EMPTY_NODE_CHECKER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_EMPTY_NODE_CHECKER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<typename ValueTraits>
struct empty_node_checker
{
  typedef ValueTraits                             value_traits;
  typedef typename value_traits::node_traits      node_traits;
  typedef typename node_traits::const_node_ptr    const_node_ptr;

  struct return_type {};

  void operator () (const_node_ptr, const return_type&, const return_type&, return_type&) {}
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_EMPTY_NODE_CHECKER_HPP_