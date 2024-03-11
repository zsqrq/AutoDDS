//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_CIRCULAR_SLIST_ALGORITHMS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_CIRCULAR_SLIST_ALGORITHMS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/uncast.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/detail/common_slist_algorithms.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

//! circular_slist_algorithms provides basic algorithms to manipulate nodes
//! forming a circular singly linked list. An empty circular list is formed by a node
//! whose pointer to the next node points to itself.
template<typename NodeTraits>
class circular_slist_algorithms : public detail::common_slist_algorithms<NodeTraits>
{
  typedef detail::common_slist_algorithms<NodeTraits> base_t;

 public:
  typedef typename NodeTraits::node            node;
  typedef typename NodeTraits::node_ptr        node_ptr;
  typedef typename NodeTraits::const_node_ptr  const_node_ptr;
  typedef NodeTraits                           node_traits;

  using base_t::transfer_after;

  //! <b>Effects</b>: Constructs an empty list, making this_node the only
  //!   node of the circular list:
  AUTODDS_INTRUSIVE_FORCEINLINE static void init_header(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_next(this_node, this_node);
  }

  //! <b>Effects</b>: Returns a pointer to a node that represents the "end" (one past end) node
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr end_node(const_node_ptr p) AUTODDS_NOEXCEPT
  {  return detail::uncast(p);   }

  //! <b>Effects</b>: Returns true if this_node_points to an empty list.
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_empty(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return NodeTraits::get_next(this_node) == this_node;
  }

  //! <b>Effects</b>: Returns true if this_node points to a sentinel node.
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_sentinel(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return NodeTraits::get_next(this_node) == node_ptr();
  }

  //! <b>Effects</b>: Marks this node as a "sentinel" node, a special state that is different from "empty",
  //!                 that can be used to mark a special state of the list
  AUTODDS_INTRUSIVE_FORCEINLINE static void set_sentinel(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_next(this_node, node_ptr());
  }

  //! <b>Effects</b>: Returns the previous node of this_node in the circular list starting.
  //!   the search from prev_init_node. The first node checked for equality
  //!   is NodeTraits::get_next(prev_init_node).
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_previous_node(node_ptr prev_init_node, node_ptr this_node) AUTODDS_NOEXCEPT
  {  return base_t::get_previous_node(prev_init_node, this_node);   }

  //! <b>Effects</b>: Returns the previous node of this_node in the circular list.
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_previous_node(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return base_t::get_previous_node(this_node, this_node);
  }

  //! <b>Effects</b>: Returns the previous node of the previous node of this_node in the circular list.
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr get_previous_previous_node(node_ptr this_node) AUTODDS_NOEXCEPT
  {  return get_previous_previous_node(this_node, this_node); }

  static node_ptr get_previous_previous_node(node_ptr p, node_ptr this_node) AUTODDS_NOEXCEPT
  {
    node_ptr p_next = NodeTraits::get_next(p);
    node_ptr p_next_next = NodeTraits::get_next(p_next);
    while (this_node != p_next_next){
      p = p_next;
      p_next = p_next_next;
      p_next_next = NodeTraits::get_next(p_next);
    }
    return p;
  }

  //! <b>Effects</b>: Returns the number of nodes in a circular list. If the circular list
  //!  is empty, returns 1.
  static std::size_t count(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    std::size_t result = 0;
    const_node_ptr p = this_node;
    do{
      p = NodeTraits::get_next(p);
      ++result;
    } while (p != this_node);
    return result;
  }

  //! <b>Effects</b>: Unlinks the node from the circular list.
  static void unlink(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    if(NodeTraits::get_next(this_node))
      base_t::unlink_after(get_previous_node(this_node));
  }

  //! <b>Effects</b>: Links this_node before nxt_node in the circular list.
  AUTODDS_INTRUSIVE_FORCEINLINE static void link_before(node_ptr nxt_node, node_ptr this_node) AUTODDS_NOEXCEPT
  {
    base_t::link_after(get_previous_node(nxt_node), this_node);
  }

  //! <b>Effects</b>: Swaps the position of the nodes: this_node is inserted in
  //!   other_nodes position in the second circular list and the other_node is inserted
  //!   in this_node's position in the first circular list.
  static void swap_nodes(node_ptr this_node, node_ptr other_node) AUTODDS_NOEXCEPT
  {
    if (other_node == this_node)
      return;
    const node_ptr this_next = NodeTraits::get_next(this_node);
    const node_ptr other_next = NodeTraits::get_next(other_node);
    const bool this_null   = !this_next;
    const bool other_null  = !other_next;
    const bool this_empty  = this_next == this_node;
    const bool other_empty = other_next == other_node;

    if(!(other_null || other_empty)){
      NodeTraits::set_next(this_next == other_node ? other_node : get_previous_node(other_node), this_node );
    }
    if(!(this_null | this_empty)){
      NodeTraits::set_next(other_next == this_node ? this_node  : get_previous_node(this_node), other_node );
    }
    NodeTraits::set_next(this_node,  other_empty ? this_node  : (other_next == this_node ? other_node : other_next) );
    NodeTraits::set_next(other_node, this_empty  ? other_node : (this_next == other_node ? this_node :  this_next ) );
  }

  //! <b>Effects</b>: Reverses the order of elements in the list.
  static void reverse(node_ptr p) AUTODDS_NOEXCEPT
  {
    node_ptr i = NodeTraits::get_next(p), e(p);
    for (;;) {
      node_ptr next(NodeTraits::get_next(i));
      if (next == e) {
        break;
      }
      base_t::transfer_after(e, i, next);
    }
  }

  //! <b>Effects</b>: Moves the node p n positions towards the end of the list.
  static node_ptr move_backwards(node_ptr p, std::size_t n) AUTODDS_NOEXCEPT
  {
    //Null shift, nothing to do
    if(!n) return node_ptr();
    node_ptr first  = NodeTraits::get_next(p);

    //count() == 1 or 2, nothing to do
    if(NodeTraits::get_next(first) == p)
      return node_ptr();

    bool end_found = false;
    node_ptr new_last = node_ptr();

    //Now find the new last node according to the shift count.
    //If we find p before finding the new last node
    //unlink p, shortcut the search now that we know the size of the list
    //and continue.
    for(std::size_t i = 1; i <= n; ++i){
      new_last = first;
      first = NodeTraits::get_next(first);
      if(first == p){
        //Shortcut the shift with the modulo of the size of the list
        n %= i;
        if(!n)
          return node_ptr();
        i = 0;
        //Unlink p and continue the new first node search
        first = NodeTraits::get_next(p);
        base_t::unlink_after(new_last);
        end_found = true;
      }
    }

    //If the p has not been found in the previous loop, find it
    //starting in the new first node and unlink it
    if(!end_found){
      base_t::unlink_after(base_t::get_previous_node(first, p));
    }

    //Now link p after the new last node
    base_t::link_after(new_last, p);
    return new_last;
  }

  //! <b>Effects</b>: Moves the node p n positions towards the beginning of the list.
  static node_ptr move_forward(node_ptr p, std::size_t n) AUTODDS_NOEXCEPT
  {
    //Null shift, nothing to do
    if(!n) return node_ptr();
    node_ptr first  = node_traits::get_next(p);

    //count() == 1 or 2, nothing to do
    if(node_traits::get_next(first) == p) return node_ptr();

    //Iterate until p is found to know where the current last node is.
    //If the shift count is less than the size of the list, we can also obtain
    //the position of the new last node after the shift.
    node_ptr old_last(first), next_to_it, new_last(p);
    std::size_t distance = 1;
    while(p != (next_to_it = node_traits::get_next(old_last))){
      if(++distance > n)
        new_last = node_traits::get_next(new_last);
      old_last = next_to_it;
    }
    //If the shift was bigger or equal than the size, obtain the equivalent
    //forward shifts and find the new last node.
    if(distance <= n){
      //Now find the equivalent forward shifts.
      //Shortcut the shift with the modulo of the size of the list
      std::size_t new_before_last_pos = (distance - (n % distance))% distance;
      //If the shift is a multiple of the size there is nothing to do
      if(!new_before_last_pos)   return node_ptr();

      for( new_last = p
          ; new_before_last_pos--
          ; new_last = node_traits::get_next(new_last)){
        //empty
      }
    }

    //Now unlink p and link it after the new last node
    base_t::unlink_after(old_last);
    base_t::link_after(new_last, p);
    return new_last;
  }

  //! <b>Effects</b>: Transfers all nodes from other after p in p's list.
  static void transfer_after(node_ptr p, node_ptr other) AUTODDS_NOEXCEPT
  {
    node_ptr other_last((get_previous_node)(other));
    base_t::transfer_after(p, other, other_last);
  }

  //! <b>Effects</b>: Unlinks all nodes reachable from p (but not p) and calls
  //!   <tt>void disposer::operator()(node_ptr)</tt> for every node of the list
  //!    where p is linked.
  template<class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE static std::size_t detach_and_dispose(node_ptr p, Disposer disposer) AUTODDS_NOEXCEPT
  {
    return base_t::unlink_after_and_dispose(p,p,disposer);
  }

};

template<typename NodeTraits>
struct get_algo<CircularSListAlgorithms, NodeTraits>
{
  typedef circular_slist_algorithms<NodeTraits> type;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_CIRCULAR_SLIST_ALGORITHMS_HPP_