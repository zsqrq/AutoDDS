//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_NODE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_NODE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_rebind.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template <typename VoidPointer>
struct slist_node
{
  typedef typename pointer_rebind<VoidPointer, slist_node>::type node_ptr;
  node_ptr next_;
};

template<class VoidPointer>
struct slist_node_traits
{
  typedef slist_node<VoidPointer>  node;
  typedef typename node::node_ptr  node_ptr;
  typedef typename pointer_rebind<VoidPointer, const node>::type    const_node_ptr;

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_next(const_node_ptr n)
  {  return n->next_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_next(node_ptr n)
  {  return n->next_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_next(node_ptr n, node_ptr next)
  {  n->next_ = next;  }
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_NODE_HPP_