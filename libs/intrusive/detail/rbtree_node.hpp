//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_RBTREE_NODE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_RBTREE_NODE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_rebind.hpp"
#include "libs/intrusive/rbtree_algorithms.hpp"
#include "libs/intrusive/pointer_plus_bits.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/tree_node.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

//This is the compact representation: 3 pointers
template<typename VoidPointer>
struct compact_rbtree_node
{
  typedef compact_rbtree_node<VoidPointer> node;
  typedef typename pointer_rebind<VoidPointer, node >::type         node_ptr;
  typedef typename pointer_rebind<VoidPointer, const node >::type   const_node_ptr;
  enum color { red_t, black_t };
  node_ptr parent_, left_, right_;
};

//This is the normal representation: 3 pointers + enum
template<typename VoidPointer>
struct rbtree_node
{
  typedef rbtree_node<VoidPointer> node;
  typedef typename pointer_rebind<VoidPointer, node >::type         node_ptr;
  typedef typename pointer_rebind<VoidPointer, const node >::type   const_node_ptr;

  enum color { red_t, black_t };
  node_ptr parent_, left_, right_;
  color color_;
};

//This is the default node traits implementation
//using a node with 3 generic pointers plus an enum
template<typename VoidPointer>
struct default_rbtree_node_traits_impl
{
  typedef rbtree_node<VoidPointer> node;
  typedef typename node::node_ptr        node_ptr;
  typedef typename node::const_node_ptr  const_node_ptr;

  typedef typename node::color color;

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

  AUTODDS_INTRUSIVE_FORCEINLINE static color get_color(const_node_ptr n)
  {  return n->color_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color get_color(node_ptr n)
  {  return n->color_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_color(node_ptr n, color c)
  {  n->color_ = c;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color black()
  {  return node::black_t;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color red()
  {  return node::red_t;  }
};

//This is the compact node traits implementation
//using a node with 3 generic pointers
template<class VoidPointer>
struct compact_rbtree_node_traits_impl
{
  typedef compact_rbtree_node<VoidPointer> node;
  typedef typename node::node_ptr        node_ptr;
  typedef typename node::const_node_ptr  const_node_ptr;

  typedef pointer_plus_bits<node_ptr, 1> ptr_bit;

  typedef typename node::color color;

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_parent(const_node_ptr n)
  {  return ptr_bit::get_pointer(n->parent_);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_parent(node_ptr n)
  {  return ptr_bit::get_pointer(n->parent_);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_parent(node_ptr n, node_ptr p)
  {  ptr_bit::set_pointer(n->parent_, p);  }

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

  AUTODDS_INTRUSIVE_FORCEINLINE static color get_color(const_node_ptr n)
  {  return (color)ptr_bit::get_bits(n->parent_);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color get_color(node_ptr n)
  {  return (color)ptr_bit::get_bits(n->parent_);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void set_color(node_ptr n, color c)
  {  ptr_bit::set_bits(n->parent_, c != 0);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color black()
  {  return node::black_t;  }

  AUTODDS_INTRUSIVE_FORCEINLINE static color red()
  {  return node::red_t;  }
};

//Dispatches the implementation based on the boolean
template<typename VoidPointer, bool Compact>
struct rbtree_node_traits_dispatch
    :  public default_rbtree_node_traits_impl<VoidPointer>
{};

template<typename VoidPointer>
struct rbtree_node_traits_dispatch<VoidPointer, true>
    :  public compact_rbtree_node_traits_impl<VoidPointer>
{};

using autodds::libs::intrusive::detail::alignment_of;
//Inherit from rbtree_node_traits_dispatch depending on the embedding capabilities
template<typename VoidPointer, bool OptimizeSize = false>
struct rbtree_node_traits
    :  public rbtree_node_traits_dispatch
        < VoidPointer
            ,  OptimizeSize &&
            (max_pointer_plus_bits
                < VoidPointer, alignment_of<compact_rbtree_node<VoidPointer> >::value
                >::value >= 1)
        >
{};


} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_RBTREE_NODE_HPP_