//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_SLIST_HPP_
#define AUTODDS_LIBS_INTRUSIVE_SLIST_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/intrusive/slist_hook.hpp"
#include "libs/intrusive/circular_slist_algorithms.hpp"
#include "libs/intrusive/linear_slist_algorithms.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/detail/get_value_traits.hpp"
#include "libs/intrusive/detail/is_stateful_value_traits.hpp"
#include "libs/intrusive/detail/default_header_holder.hpp"
#include "libs/intrusive/detail/uncast.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/iterator.hpp"
#include "libs/intrusive/detail/slist_iterator.hpp"
#include "libs/intrusive/detail/array_initializer.hpp"
#include "libs/intrusive/detail/exception_disposer.hpp"
#include "libs/intrusive/detail/equal_to_value.hpp"
#include "libs/intrusive/detail/key_nodeptr_comp.hpp"
#include "libs/intrusive/detail/simple_disposers.hpp"
#include "libs/intrusive/detail/size_holder.hpp"
#include "libs/intrusive/detail/algorithm.hpp"
#include "libs/intrusive/detail/value_functors.hpp"
#include "libs/intrusive/detail/node_cloner_disposer.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/common/static_assert.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

template<typename HeaderHolder, typename NodePtr, bool>
struct header_holder_plus_last
{
  HeaderHolder header_holder_;
  NodePtr  last_;
};

template<typename HeaderHolder, typename NodePtr>
struct header_holder_plus_last<HeaderHolder, NodePtr, false>
{
  HeaderHolder header_holder_;
};

struct default_slist_hook_applier
{
  template<class T>
  struct apply
  {
    typedef typename T::default_slist_hook type;
  };
};

template<>
struct is_default_hook_tag<default_slist_hook_applier>
{  static const bool value = true;  };

struct slist_defaults
{
  typedef default_slist_hook_applier proto_value_traits;
  static const bool constant_time_size = true;
  static const bool linear = false;
  typedef std::size_t size_type;
  static const bool cache_last = false;
  typedef void header_holder_type;
};

struct slist_bool_flags
{
  static const std::size_t linear_pos             = 1u;
  static const std::size_t constant_time_size_pos = 2u;
  static const std::size_t cache_last_pos         = 4u;
};

//! The class template slist is an intrusive container, that encapsulates
//! a singly-linked list. You can use such a list to squeeze the last bit
//! of performance from your application. Unfortunately, the little gains
//! come with some huge drawbacks. A lot of member functions can't be
//! implemented as efficiently as for standard containers. To overcome
//! this limitation some other member functions with rather unusual semantics
//! have to be introduced.

template<typename ValueTraits, typename SizeType, std::size_t BoolFlags, typename HeaderHolder>
class slist_impl
{
 public:
  typedef ValueTraits                                               value_traits;
  typedef typename value_traits::pointer                            pointer;
  typedef typename value_traits::const_pointer                      const_pointer;
  typedef typename pointer_traits<pointer>::element_type            value_type;
  typedef typename pointer_traits<pointer>::reference               reference;
  typedef typename pointer_traits<const_pointer>::reference         const_reference;
  typedef typename pointer_traits<pointer>::difference_type         difference_type;
  typedef SizeType                                                  size_type;
  typedef slist_iterator<value_traits, false>                       iterator;
  typedef slist_iterator<value_traits, true>                        const_iterator;
  typedef typename value_traits::node_traits                        node_traits;
  typedef typename node_traits::node                                node;
  typedef typename node_traits::node_ptr                            node_ptr;
  typedef typename node_traits::const_node_ptr                      const_node_ptr;
  typedef typename detail::get_header_holder_type
      < value_traits, HeaderHolder >::type                           header_holder_type;

  static const bool constant_time_size = 0 != (BoolFlags & slist_bool_flags::constant_time_size_pos);
  static const bool stateful_value_traits = detail::is_stateful_value_traits<value_traits>::value;
  static const bool linear = 0 != (BoolFlags & slist_bool_flags::linear_pos);
  static const bool cache_last = 0 != (BoolFlags & slist_bool_flags::cache_last_pos);
  static const bool has_container_from_iterator =
      detail::is_same< header_holder_type, detail::default_header_holder< node_traits > >::value;

  typedef typename detail::if_c
      < linear, linear_slist_algorithms<node_traits>,
        circular_slist_algorithms<node_traits>
      >::type                                                        node_algorithms;

 private:
  typedef detail::size_holder<constant_time_size, size_type>    size_traits;

  AUTODDS_MOVABLE_BUT_NOT_COPYABLE(slist_impl);

  static const bool safemode_or_autolink = is_safe_autounlink<value_traits::link_mode>::value;

  //Constant-time size is incompatible with auto-unlink hooks!
  AUTODDS_STATIC_ASSERT(!(constant_time_size && ((int)value_traits::link_mode == (int)auto_unlink)));
  //Linear singly linked lists are incompatible with auto-unlink hooks!
  AUTODDS_STATIC_ASSERT(!(linear && ((int)value_traits::link_mode == (int)auto_unlink)));
  //A list with cached last node is incompatible with auto-unlink hooks!
  AUTODDS_STATIC_ASSERT(!(cache_last && ((int)value_traits::link_mode == (int)auto_unlink)));

};

} // namespace intrusive
} // namespace libs
} // namespace intrusive


#endif //AUTODDS_LIBS_INTRUSIVE_SLIST_HPP_