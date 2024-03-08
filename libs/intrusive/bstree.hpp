//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/common/static_assert.hpp"
#include "libs/intrusive/bs_set_hook.hpp"
#include "libs/intrusive/detail/tree_node.hpp"
#include "libs/intrusive/detail/tree_iterator.hpp"
#include "libs/intrusive/detail/ebo_functor_holder.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/is_stateful_value_traits.hpp"
#include "libs/intrusive/detail/empty_node_checker.hpp"
#include "libs/intrusive/detail/default_header_holder.hpp"
#include "libs/intrusive/detail/reverse_iterator.hpp"
#include "libs/intrusive/detail/exception_disposer.hpp"
#include "libs/intrusive/detail/node_cloner_disposer.hpp"
#include "libs/intrusive/detail/key_nodeptr_comp.hpp"
#include "libs/intrusive/detail/simple_disposers.hpp"
#include "libs/intrusive/detail/size_holder.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/detail/algorithm.hpp"
#include "libs/intrusive/detail/tree_value_compare.hpp"
#include "libs/intrusive/detail/get_value_traits.hpp"
#include "libs/intrusive/bstree_algorithms.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/parent_from_member.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/move/adl_move_swap.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

struct default_bstree_hook_applier
{
  template<class T>
  struct apply
  {
    typedef typename T::default_bstree_hook type;
  };
};

template<>
struct is_default_hook_tag<default_bstree_hook_applier>
{
  static const bool value = true;
};

struct bstree_defaults
{
  typedef default_bstree_hook_applier   proto_value_traits;
  static const bool constant_time_size = true;
  typedef std::size_t size_type;
  typedef void compare;
  typedef void key_of_value;
  static const bool floating_point = true;  //For sgtree
  typedef void priority;  //For treap
  typedef void header_holder_type;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_