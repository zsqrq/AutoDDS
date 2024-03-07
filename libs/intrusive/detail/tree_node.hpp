//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_NODE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_NODE_HPP_

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

template<typename VoidPointer>
struct tree_node
{
  typedef typename pointer_rebind<VoidPointer, tree_node>::type  node_ptr;

  node_ptr parent_, left_, right_;
};

template<class VoidPointer>
struct tree_node_traits
{
  typedef tree_node<VoidPointer> node;

  typedef typename node::node_ptr   node_ptr;
  typedef typename pointer_rebind<VoidPointer, const node>::type const_node_ptr;

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_parent(const_node_ptr n)
  {  return n->parent_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_parent(node_ptr n)
  {  return n->parent_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_parent(node_ptr n, node_ptr p)
  {  n->parent_ = p;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_left(const_node_ptr n)
  {  return n->left_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_left(node_ptr n)
  {  return n->left_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_left(node_ptr n, node_ptr l)
  {  n->left_ = l;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_right(const_node_ptr n)
  {  return n->right_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_right(node_ptr n)
  {  return n->right_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_right(node_ptr n, node_ptr r)
  {  n->right_ = r;  }
};

} // namespace intrusive
} // namespace libs
} // namespce autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_NODE_HPP_