//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_LINEAR_SLIST_ALGORITHMS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_LINEAR_SLIST_ALGORITHMS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/common_slist_algorithms.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/detail/twin.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

template<typename NodeTraits>
class linear_slist_algorithms : public detail::common_slist_algorithms<NodeTraits>
{
  typedef detail::common_slist_algorithms<NodeTraits>  base_t;
 public:
  typedef typename NodeTraits::node              node;
  typedef typename NodeTraits::node_ptr          node_ptr;
  typedef typename NodeTraits::const_node_ptr    const_node_ptr;
  typedef NodeTraits                             node_traits;
  typedef twin<node_ptr>                         node_pair;
  
  using base_t::transfer_after;
  
  AUTODDS_INTRUSIVE_FORCEINLINE static void init_header(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_next(this_node, node_ptr());
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr end_node(const_node_ptr) AUTODDS_NOEXCEPT
  {
    return node_ptr();
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_empty(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return !NodeTraits::get_next(this_node);
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_sentinel(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return NodeTraits::get_next(this_node) == this_node;
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE static void set_sentinel(node_ptr this_node) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_next(this_node, this_node);
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr 
  get_previous_node(node_ptr prev_init_node, node_ptr this_node) AUTODDS_NOEXCEPT
  {
    return base_t::get_previous_node(prev_init_node, this_node);
  }
  
  static std::size_t count(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    std::size_t result = 0;
    const_node_ptr p = this_node;
    
    do {
      p = NodeTraits::get_next(p);
      ++result;
    } while (p);
    return result;
  }

  //! <b>Effects</b>: Moves all the nodes previously chained after this_node after other_node
  AUTODDS_INTRUSIVE_FORCEINLINE static void swap_trailing_nodes(node_ptr this_node, node_ptr other_node) AUTODDS_NOEXCEPT
  {
    node_ptr this_nxt    = NodeTraits::get_next(this_node);
    node_ptr other_nxt   = NodeTraits::get_next(other_node);
    NodeTraits::set_next(this_node, other_nxt);
    NodeTraits::set_next(other_node, this_nxt);
  }

  static node_ptr reverse(node_ptr p) AUTODDS_NOEXCEPT
  {
    if(!p) return node_ptr();
    node_ptr i = NodeTraits::get_next(p);
    node_ptr first(p);
    while(i){
      node_ptr nxti(NodeTraits::get_next(i));
      base_t::unlink_after(p);
      NodeTraits::set_next(i, first);
      first = i;
      i = nxti;
    }
    return first;
  }

  //! <b>Effects</b>: Moves the first n nodes starting at p to the end of the list.
  static node_pair move_first_n_backwards(node_ptr p, std::size_t n) AUTODDS_NOEXCEPT
  {
    node_pair ret;
    //Null shift, or count() == 0 or 1, nothing to do
    if(!n || !p || !NodeTraits::get_next(p)){
      return ret;
    }

    node_ptr first = p;
    bool end_found = false;
    node_ptr new_last = node_ptr();
    node_ptr old_last = node_ptr();

    //Now find the new last node according to the shift count.
    //If we find 0 before finding the new last node
    //unlink p, shortcut the search now that we know the size of the list
    //and continue.
    for(std::size_t i = 1; i <= n; ++i){
      new_last = first;
      first = NodeTraits::get_next(first);
      if(first == node_ptr()){
        //Shortcut the shift with the modulo of the size of the list
        n %= i;
        if(!n)   return ret;
        old_last = new_last;
        i = 0;
        //Unlink p and continue the new first node search
        first = p;
        //unlink_after(new_last);
        end_found = true;
      }
    }

    //If the p has not been found in the previous loop, find it
    //starting in the new first node and unlink it
    if(!end_found){
      old_last = base_t::get_previous_node(first, node_ptr());
    }

    //Now link p after the new last node
    NodeTraits::set_next(old_last, p);
    NodeTraits::set_next(new_last, node_ptr());
    ret.first   = first;
    ret.second  = new_last;
    return ret;
  }

  static node_pair move_first_n_forward(node_ptr p, std::size_t n) AUTODDS_NOEXCEPT
  {
    node_pair ret;
    //Null shift, or count() == 0 or 1, nothing to do
    if(!n || !p || !NodeTraits::get_next(p))
      return ret;

    node_ptr first  = p;

    //Iterate until p is found to know where the current last node is.
    //If the shift count is less than the size of the list, we can also obtain
    //the position of the new last node after the shift.
    node_ptr old_last(first), next_to_it, new_last(p);
    std::size_t distance = 1;
    while(!!(next_to_it = node_traits::get_next(old_last))){
      if(distance++ > n)
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
      if(!new_before_last_pos)
        return ret;

      for( new_last = p
          ; --new_before_last_pos
          ; new_last = node_traits::get_next(new_last)){
        //empty
      }
    }

    //Get the first new node
    node_ptr new_first(node_traits::get_next(new_last));
    //Now put the old beginning after the old end
    NodeTraits::set_next(old_last, p);
    NodeTraits::set_next(new_last, node_ptr());
    ret.first   = new_first;
    ret.second  = new_last;
    return ret;
  }

  //! <b>Effects</b>: Transfers all nodes from other after p in p's linear list.
  static void transfer_after(node_ptr p, node_ptr other) AUTODDS_NOEXCEPT
  {
    if ((is_empty)(p)) {
      (swap_trailing_nodes)(p, other);
    }
    else {
      node_ptr other_last((get_previous_node)(other, node_ptr()));
      base_t::transfer_after(p, other, other_last);
    }
  }
  template<class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE static std::size_t detach_and_dispose(node_ptr p, Disposer disposer) AUTODDS_NOEXCEPT
  {  return base_t::unlink_after_and_dispose(p, node_ptr(), disposer);   }
  
};

template<typename NodeTraits>
struct get_algo<LinearSListAlgorithms, NodeTraits>
{
  typedef linear_slist_algorithms<NodeTraits> type;
};

} // namespace intrusive 
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_LINEAR_SLIST_ALGORITHMS_HPP_