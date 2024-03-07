//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_BSTREE_ALGORITHMS_BASE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_BSTREE_ALGORITHMS_BASE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/uncast.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename NodeTraits>
class bstree_algorithms_base
{
 public:
  typedef typename NodeTraits::node       node;
  typedef NodeTraits                      node_traits;
  typedef typename NodeTraits::node_ptr   node_ptr;
  typedef typename NodeTraits::const_node_ptr  const_node_ptr;

  static node_ptr next_node(node_ptr n) AUTODDS_NOEXCEPT
  {
    node_ptr const n_right(NodeTraits::get_right(n));
    if (n_right) {
      return minimum(n_right);
    } else {
      node_ptr p(NodeTraits::get_parent(n));
      while (n == NodeTraits::get_right(p)) {
        n = p;
        p = NodeTraits::get_parent(p);
      }
      return NodeTraits::get_right(n) != p ? p : n;
    }
  }

  static node_ptr prev_node(node_ptr n) AUTODDS_NOEXCEPT
  {
    if (is_header(n)) {
      return NodeTraits::get_right(n);
    }
    else if (NodeTraits::get_left(n)) {
      return maximum(NodeTraits::get_left(n));
    } else {
      node_ptr p(n);
      node_ptr x = NodeTraits::get_parent(p);
      while(p == NodeTraits::get_left(x)){
        p = x;
        x = NodeTraits::get_parent(x);
      }
      return x;
    }

  }

  static node_ptr minimum(node_ptr n)
  {
    for(node_ptr p_left = NodeTraits::get_left(n)
        ;p_left
        ;p_left = NodeTraits::get_left(n)){
      n = p_left;
    }
    return n;
  }

  static node_ptr maximum(node_ptr n)
  {
    for(node_ptr p_right = NodeTraits::get_right(n)
        ;p_right
        ;p_right = NodeTraits::get_right(n)){
      n = p_right;
    }
    return n;
  }

  static bool is_header(const_node_ptr p) AUTODDS_NOEXCEPT
  {
    node_ptr p_left (NodeTraits::get_left(p));
    node_ptr p_right(NodeTraits::get_right(p));
    if(!NodeTraits::get_parent(p) || //Header condition when empty tree
        (p_left && p_right &&         //Header always has leftmost and rightmost
            (p_left == p_right ||      //Header condition when only node
                (NodeTraits::get_parent(p_left)  != p ||
                    NodeTraits::get_parent(p_right) != p ))
            //When tree size > 1 headers can't be leftmost's
            //and rightmost's parent
        )){
      return true;
    }
    return false;
  }

  static node_ptr get_header(const_node_ptr n)
  {
    node_ptr nn(detail::uncast(n));
    node_ptr p(NodeTraits::get_parent(n));
    //If p is null, then nn is the header of an empty tree
    if(p){
      //Non-empty tree, check if nn is neither root nor header
      node_ptr pp(NodeTraits::get_parent(p));
      //If granparent is not equal to nn, then nn is neither root nor header,
      //the try the fast path
      if(nn != pp){
        do{
          nn = p;
          p = pp;
          pp = NodeTraits::get_parent(pp);
        }while(nn != pp);
        nn = p;
      }
        //Check if nn is root or header when size() > 0
      else if(!bstree_algorithms_base::is_header(nn)){
        nn = p;
      }
    }
    return nn;
  }
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_BSTREE_ALGORITHMS_BASE_HPP_