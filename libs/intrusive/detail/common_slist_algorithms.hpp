//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_COMMON_SLIST_ALGORITHMS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_COMMON_SLIST_ALGORITHMS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<typename NodeTraits>
class common_slist_algorithms
{
 public:
  typedef typename NodeTraits::node            node;
  typedef typename NodeTraits::node_ptr        node_ptr;
  typedef typename NodeTraits::const_node_ptr  const_node_ptr;
  typedef NodeTraits                           node_traits;

  static node_ptr get_previous_node(node_ptr p, node_ptr this_node)
  {
    for( node_ptr p_next
        ; this_node != (p_next = NodeTraits::get_next(p))
        ; p = p_next){
      //Logic error: possible use of linear lists with
      //operations only permitted with circular lists
      AUTODDS_INTRUSIVE_INVARIANT_ASSERT(p);
    }
    return p;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void init(node_ptr this_node) AUTODDS_NOEXCEPT
  {  NodeTraits::set_next(this_node, node_ptr());  }

  static bool unique(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {
    node_ptr next = NodeTraits::get_next(this_node);
    return !next || next == this_node;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static bool inited(const_node_ptr this_node) AUTODDS_NOEXCEPT
  {  return !NodeTraits::get_next(this_node); }

  AUTODDS_INTRUSIVE_FORCEINLINE static void unlink_after(node_ptr prev_node) AUTODDS_NOEXCEPT
  {
    const_node_ptr this_node(NodeTraits::get_next(prev_node));
    NodeTraits::set_next(prev_node, NodeTraits::get_next(this_node));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static void unlink_after(node_ptr prev_node, node_ptr last_node) AUTODDS_NOEXCEPT
  {  NodeTraits::set_next(prev_node, last_node);  }

  static void link_after(node_ptr prev_node, node_ptr this_node) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_next(this_node, NodeTraits::get_next(prev_node));
    NodeTraits::set_next(prev_node, this_node);
  }

  static void incorporate_after(node_ptr bp, node_ptr b, node_ptr be) AUTODDS_NOEXCEPT
  {
    node_ptr p(NodeTraits::get_next(bp));
    NodeTraits::set_next(bp, b);
    NodeTraits::set_next(be, p);
  }

  static void transfer_after(node_ptr bp, node_ptr bb, node_ptr be) AUTODDS_NOEXCEPT
  {
    if (bp != bb && bp != be && bb != be) {
      node_ptr next_b = NodeTraits::get_next(bb);
      node_ptr next_e = NodeTraits::get_next(be);
      node_ptr next_p = NodeTraits::get_next(bp);
      NodeTraits::set_next(bb, next_e);
      NodeTraits::set_next(be, next_p);
      NodeTraits::set_next(bp, next_b);
    }
  }

  struct stable_partition_info
  {
    std::size_t num_1st_partition;
    std::size_t num_2nd_partition;
    node_ptr    beg_2st_partition;
    node_ptr    new_last_node;
  };

  template<class Pred>
  static void stable_partition(node_ptr before_beg, node_ptr end, Pred pred, stable_partition_info &info)
  {
    node_ptr bcur = before_beg;
    node_ptr cur  = node_traits::get_next(bcur);
    node_ptr new_f = end;

    std::size_t num1 = 0, num2 = 0;
    while(cur != end){
      if(pred(cur)){
        ++num1;
        bcur = cur;
        cur  = node_traits::get_next(cur);
      }
      else{
        ++num2;
        node_ptr last_to_remove = bcur;
        new_f = cur;
        bcur = cur;
        cur  = node_traits::get_next(cur);
        AUTODDS_INTRUSIVE_TRY{
            //Main loop
            while(cur != end){
              if(pred(cur)){ //Might throw
                ++num1;
                //Process current node
                node_traits::set_next(last_to_remove, cur);
                last_to_remove = cur;
                node_ptr nxt = node_traits::get_next(cur);
                node_traits::set_next(bcur, nxt);
                cur = nxt;
              }
              else{
                ++num2;
                bcur = cur;
                cur  = node_traits::get_next(cur);
              }
            }
        }
        AUTODDS_INTRUSIVE_CATCH(...){
          node_traits::set_next(last_to_remove, new_f);
          AUTODDS_INTRUSIVE_RETHROW;
        }
        AUTODDS_INTRUSIVE_CATCH_END
            node_traits::set_next(last_to_remove, new_f);
        break;
      }
    }
    info.num_1st_partition = num1;
    info.num_2nd_partition = num2;
    info.beg_2st_partition = new_f;
    info.new_last_node = bcur;
  }

  //! <b>Effects</b>: Returns the number of nodes in the range [f, l).
  static std::size_t distance(const_node_ptr f, const_node_ptr l) AUTODDS_NOEXCEPT
  {
    const_node_ptr i(f);
    std::size_t result = 0;
    while (i != l)
    {
      i = NodeTraits::get_next(i);
      ++result;
    }
    return result;
  }

  template<class Disposer>
  static std::size_t unlink_after_and_dispose(node_ptr bb, node_ptr e, Disposer disposer) AUTODDS_NOEXCEPT
  {
    std::size_t n = 0u;
    node_ptr i = node_traits::get_next(bb);
    while (i != e) {
      node_ptr to_erase(i);
      i = node_traits::get_next(i);
      disposer(to_erase);
      ++n;
    }
    node_traits::set_next(bb, e);
    return n;
  }

  template<class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE static void unlink_after_and_dispose(node_ptr bb, Disposer disposer) AUTODDS_NOEXCEPT
  {
    node_ptr i = node_traits::get_next(bb);
    node_traits::set_next(bb, node_traits::get_next(i));
    disposer(i);
  }

  template<class Disposer>
  static std::size_t detach_and_dispose(node_ptr p, Disposer disposer) AUTODDS_NOEXCEPT
  {
    std::size_t n = 0;
    node_ptr i = node_traits::get_next(p);
    while ( i != p || i != node_ptr() ) {
      node_ptr to_erase(i);
      i = node_traits::get_next(i);
      disposer(to_erase);
    }
    node_traits::set_next(p, i);
    return n;
  }
  
};

} // namespace detail

template<typename NodeTraits>
struct get_algo<CommonSListAlgorithms, NodeTraits>
{
  typedef detail::common_slist_algorithms<NodeTraits> type;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_COMMON_SLIST_ALGORITHMS_HPP_