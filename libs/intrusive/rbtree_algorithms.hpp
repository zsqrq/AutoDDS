//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_RBTREE_ALGORITHMS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_RBTREE_ALGORITHMS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/assert.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/bstree_algorithms.hpp"
#include "libs/intrusive/detail/ebo_functor_holder.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename NodeTraits, typename F>
struct rbtree_node_cloner :  public detail::ebo_functor_holder<F>
{
  typedef typename NodeTraits::node_ptr  node_ptr;
  typedef detail::ebo_functor_holder<F>  base_t;

  explicit rbtree_node_cloner(F f)
      :  base_t(f)
  {}

  node_ptr operator()(node_ptr p)
  {
    node_ptr n = base_t::get()(p);
    NodeTraits::set_color(n, NodeTraits::get_color(p));
    return n;
  }
};

namespace detail {

template<typename ValueTraits, typename NodePtrCompare, typename ExtraChecker>
struct rbtree_node_checker
    : public bstree_node_checker<ValueTraits, NodePtrCompare, ExtraChecker>
{
  typedef bstree_node_checker<ValueTraits, NodePtrCompare, ExtraChecker> base_checker_t;
  typedef ValueTraits                             value_traits;
  typedef typename value_traits::node_traits      node_traits;
  typedef typename node_traits::const_node_ptr    const_node_ptr;
  typedef typename node_traits::node_ptr          node_ptr;

  struct return_type
      : public base_checker_t::return_type
  {
    return_type() : black_count_(0) {}
    std::size_t black_count_;
  };

  rbtree_node_checker(const NodePtrCompare& comp, ExtraChecker extra_checker)
      : base_checker_t(comp, extra_checker)
  {}

  void operator () (const_node_ptr p,
                    const return_type& check_return_left, const return_type& check_return_right,
                    return_type& check_return)
  {

    if (node_traits::get_color(p) == node_traits::red()){
      //Red nodes have black children
      const node_ptr p_left(node_traits::get_left(p));   (void)p_left;
      const node_ptr p_right(node_traits::get_right(p)); (void)p_right;
      AUTODDS_INTRUSIVE_INVARIANT_ASSERT(!p_left  || node_traits::get_color(p_left)  == node_traits::black());
      AUTODDS_INTRUSIVE_INVARIANT_ASSERT(!p_right || node_traits::get_color(p_right) == node_traits::black());
      //Red node can't be root
      AUTODDS_INTRUSIVE_INVARIANT_ASSERT(node_traits::get_parent(node_traits::get_parent(p)) != p);
    }
    //Every path to p contains the same number of black nodes
    const std::size_t l_black_count = check_return_left.black_count_;
    AUTODDS_INTRUSIVE_INVARIANT_ASSERT(l_black_count == check_return_right.black_count_);
    check_return.black_count_ = l_black_count +
        static_cast<std::size_t>(node_traits::get_color(p) == node_traits::black());
    base_checker_t::operator()(p, check_return_left, check_return_right, check_return);
  }
};

} // namespace detail

//! rbtree_algorithms provides basic algorithms to manipulate
//! nodes forming a red-black tree. The insertion and deletion algorithms are
//! based on those in Cormen, Leiserson, and Rivest, Introduction to Algorithms
//! (MIT Press, 1990), except that
template <typename NodeTraits>
class rbtree_algorithms : public bstree_algorithms<NodeTraits>
{
 public:
  typedef NodeTraits                           node_traits;
  typedef typename NodeTraits::node            node;
  typedef typename NodeTraits::node_ptr        node_ptr;
  typedef typename NodeTraits::const_node_ptr  const_node_ptr;
  typedef typename NodeTraits::color           color;

 private:

  typedef bstree_algorithms<NodeTraits>  bstree_algo;
  
 public:

  typedef typename bstree_algo::insert_commit_data insert_commit_data;

  static void swap_nodes(node_ptr node1, node_ptr node2) AUTODDS_NOEXCEPT
  {
    if(node1 == node2)
      return;

    node_ptr header1(bstree_algo::get_header(node1)), header2(bstree_algo::get_header(node2));
    swap_nodes(node1, header1, node2, header2);
  }

  static void swap_nodes(node_ptr node1, node_ptr header1, node_ptr node2, node_ptr header2) AUTODDS_NOEXCEPT
  {
    if(node1 == node2)   return;

    bstree_algo::swap_nodes(node1, header1, node2, header2);
    //Swap color
    color c = NodeTraits::get_color(node1);
    NodeTraits::set_color(node1, NodeTraits::get_color(node2));
    NodeTraits::set_color(node2, c);
  }

  static void replace_node(node_ptr node_to_be_replaced, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    if(node_to_be_replaced == new_node)
      return;
    replace_node(node_to_be_replaced, bstree_algo::get_header(node_to_be_replaced), new_node);
  }

  static void replace_node(node_ptr node_to_be_replaced, node_ptr header, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    bstree_algo::replace_node(node_to_be_replaced, header, new_node);
    NodeTraits::set_color(new_node, NodeTraits::get_color(node_to_be_replaced));
  }

  static void unlink(node_ptr n) AUTODDS_NOEXCEPT
  {
    node_ptr x = NodeTraits::get_parent(n);
    if(x){
      while(!is_header(x))
        x = NodeTraits::get_parent(x);
      erase(x, n);
    }
  }

  static void init_header(node_ptr header) AUTODDS_NOEXCEPT
  {
    bstree_algo::init_header(header);
    NodeTraits::set_color(header, NodeTraits::red());
  }

  static node_ptr erase(node_ptr header, node_ptr z) AUTODDS_NOEXCEPT
  {
    typename bstree_algo::data_for_rebalance info;
    bstree_algo::erase(header, z, info);
    rebalance_after_erasure(header, z, info);
    return z;
  }

  template<typename NodePtrCompare>
  static bool transfer_unique
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z)
  {
    typename bstree_algo::data_for_rebalance info;
    bool const transferred = bstree_algo::transfer_unique(header1, comp, header2, z, info);
    if(transferred){
      rebalance_after_erasure(header2, z, info);
      rebalance_after_insertion(header1, z);
    }
    return transferred;
  }

  template<typename NodePtrCompare>
  static void transfer_equal
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z)
  {
    typename bstree_algo::data_for_rebalance info;
    bstree_algo::transfer_equal(header1, comp, header2, z, info);
    rebalance_after_erasure(header2, z, info);
    rebalance_after_insertion(header1, z);
  }

  template <typename Cloner, typename Disposer>
  static void clone(const_node_ptr source_header, 
                    node_ptr target_header, Cloner cloner, Disposer disposer)
  {
    rbtree_node_cloner<NodeTraits, Cloner> new_cloner(cloner);
    bstree_algo::clone(source_header, target_header, new_cloner, disposer);
  }

  template<typename NodePtrCompare>
  static node_ptr insert_equal_upper_bound(node_ptr h, node_ptr new_node, NodePtrCompare comp)
  {
    bstree_algo::insert_equal_upper_bound(h, new_node, comp);
    rebalance_after_insertion(h, new_node);
    return new_node;
  }

  template<typename NodePtrCompare>
  static node_ptr insert_equal_lower_bound(node_ptr h, node_ptr new_node, NodePtrCompare comp)
  {
    bstree_algo::insert_equal_lower_bound(h, new_node, comp);
    rebalance_after_insertion(h, new_node);
    return new_node;
  }

  template<typename NodePtrCompare>
  static node_ptr insert_equal(node_ptr header, node_ptr hint, node_ptr new_node, NodePtrCompare comp)
  {
    bstree_algo::insert_equal(header, hint, new_node, comp);
    rebalance_after_insertion(header, new_node);
    return new_node;
  }

  static node_ptr insert_before(node_ptr header, node_ptr pos, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    bstree_algo::insert_before(header, pos, new_node);
    rebalance_after_insertion(header, new_node);
    return new_node;
  }

  static void push_back(node_ptr header, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    bstree_algo::push_back(header, new_node);
    rebalance_after_insertion(header, new_node);
  }

  static void push_front(node_ptr header, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    bstree_algo::push_front(header, new_node);
    rebalance_after_insertion(header, new_node);
  }

  static void insert_unique_commit(node_ptr header, node_ptr new_value, 
                                   const insert_commit_data &commit_data) AUTODDS_NOEXCEPT
  {
    bstree_algo::insert_unique_commit(header, new_value, commit_data);
    rebalance_after_insertion(header, new_value);
  }

  static bool is_header(const_node_ptr p) AUTODDS_NOEXCEPT
  {
    return NodeTraits::get_color(p) == NodeTraits::red() &&
        bstree_algo::is_header(p);
  }
  
 private:

  static void rebalance_after_erasure( node_ptr header, node_ptr z, 
                                       const typename bstree_algo::data_for_rebalance &info) AUTODDS_NOEXCEPT
  {
    color new_z_color;
    if(info.y != z){
      new_z_color = NodeTraits::get_color(info.y);
      NodeTraits::set_color(info.y, NodeTraits::get_color(z));
    }
    else{
      new_z_color = NodeTraits::get_color(z);
    }
    //Rebalance rbtree if needed
    if(new_z_color != NodeTraits::red()){
      rebalance_after_erasure_restore_invariants(header, info.x, info.x_parent);
    }
  }

  static void rebalance_after_erasure_restore_invariants(node_ptr header, node_ptr x, node_ptr x_parent) AUTODDS_NOEXCEPT
  {
    while(1){
      if(x_parent == header || (x && NodeTraits::get_color(x) != NodeTraits::black())){
        break;
      }
      //Don't cache x_is_leftchild or similar because x can be null and
      //equal to both x_parent_left and x_parent_right
      const node_ptr x_parent_left(NodeTraits::get_left(x_parent));
      if(x == x_parent_left){ //x is left child
        node_ptr w = NodeTraits::get_right(x_parent);
        AUTODDS_INTRUSIVE_INVARIANT_ASSERT(w);
        if(NodeTraits::get_color(w) == NodeTraits::red()){
          NodeTraits::set_color(w, NodeTraits::black());
          NodeTraits::set_color(x_parent, NodeTraits::red());
          bstree_algo::rotate_left(x_parent, w, NodeTraits::get_parent(x_parent), header);
          w = NodeTraits::get_right(x_parent);
          AUTODDS_INTRUSIVE_INVARIANT_ASSERT(w);
        }
        node_ptr const w_left (NodeTraits::get_left(w));
        node_ptr const w_right(NodeTraits::get_right(w));
        if((!w_left  || NodeTraits::get_color(w_left)  == NodeTraits::black()) &&
            (!w_right || NodeTraits::get_color(w_right) == NodeTraits::black())){
          NodeTraits::set_color(w, NodeTraits::red());
          x = x_parent;
          x_parent = NodeTraits::get_parent(x_parent);
        }
        else {
          if(!w_right || NodeTraits::get_color(w_right) == NodeTraits::black()){
            NodeTraits::set_color(w_left, NodeTraits::black());
            NodeTraits::set_color(w, NodeTraits::red());
            bstree_algo::rotate_right(w, w_left, NodeTraits::get_parent(w), header);
            w = NodeTraits::get_right(x_parent);
            AUTODDS_INTRUSIVE_INVARIANT_ASSERT(w);
          }
          NodeTraits::set_color(w, NodeTraits::get_color(x_parent));
          NodeTraits::set_color(x_parent, NodeTraits::black());
          const node_ptr new_wright(NodeTraits::get_right(w));
          if(new_wright)
            NodeTraits::set_color(new_wright, NodeTraits::black());
          bstree_algo::rotate_left(x_parent, NodeTraits::get_right(x_parent), NodeTraits::get_parent(x_parent), header);
          break;
        }
      }
      else {
        // same as above, with right_ <-> left_.
        node_ptr w = x_parent_left;
        if(NodeTraits::get_color(w) == NodeTraits::red()){
          NodeTraits::set_color(w, NodeTraits::black());
          NodeTraits::set_color(x_parent, NodeTraits::red());
          bstree_algo::rotate_right(x_parent, w, NodeTraits::get_parent(x_parent), header);
          w = NodeTraits::get_left(x_parent);
          AUTODDS_INTRUSIVE_INVARIANT_ASSERT(w);
        }
        node_ptr const w_left (NodeTraits::get_left(w));
        node_ptr const w_right(NodeTraits::get_right(w));
        if((!w_right || NodeTraits::get_color(w_right) == NodeTraits::black()) &&
            (!w_left  || NodeTraits::get_color(w_left)  == NodeTraits::black())){
          NodeTraits::set_color(w, NodeTraits::red());
          x = x_parent;
          x_parent = NodeTraits::get_parent(x_parent);
        }
        else {
          if(!w_left || NodeTraits::get_color(w_left) == NodeTraits::black()){
            NodeTraits::set_color(w_right, NodeTraits::black());
            NodeTraits::set_color(w, NodeTraits::red());
            bstree_algo::rotate_left(w, w_right, NodeTraits::get_parent(w), header);
            w = NodeTraits::get_left(x_parent);
            AUTODDS_INTRUSIVE_INVARIANT_ASSERT(w);
          }
          NodeTraits::set_color(w, NodeTraits::get_color(x_parent));
          NodeTraits::set_color(x_parent, NodeTraits::black());
          const node_ptr new_wleft(NodeTraits::get_left(w));
          if(new_wleft)
            NodeTraits::set_color(new_wleft, NodeTraits::black());
          bstree_algo::rotate_right(x_parent, NodeTraits::get_left(x_parent), NodeTraits::get_parent(x_parent), header);
          break;
        }
      }
    }
    if(x)
      NodeTraits::set_color(x, NodeTraits::black());
  }

  static void rebalance_after_insertion(node_ptr header, node_ptr p) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_color(p, NodeTraits::red());
    while(1){
      node_ptr p_parent(NodeTraits::get_parent(p));
      const node_ptr p_grandparent(NodeTraits::get_parent(p_parent));
      if(p_parent == header || NodeTraits::get_color(p_parent) == NodeTraits::black() || p_grandparent == header){
        break;
      }

      NodeTraits::set_color(p_grandparent, NodeTraits::red());
      node_ptr const p_grandparent_left (NodeTraits::get_left (p_grandparent));
      bool const p_parent_is_left_child = p_parent == p_grandparent_left;
      node_ptr const x(p_parent_is_left_child ? NodeTraits::get_right(p_grandparent) : p_grandparent_left);

      if(x && NodeTraits::get_color(x) == NodeTraits::red()){
        NodeTraits::set_color(x, NodeTraits::black());
        NodeTraits::set_color(p_parent, NodeTraits::black());
        p = p_grandparent;
      }
      else{ //Final step
        const bool p_is_left_child(NodeTraits::get_left(p_parent) == p);
        if(p_parent_is_left_child){ //p_parent is left child
          if(!p_is_left_child){ //p is right child
            bstree_algo::rotate_left_no_parent_fix(p_parent, p);
            p_parent = p;
          }
          bstree_algo::rotate_right(p_grandparent, p_parent, NodeTraits::get_parent(p_grandparent), header);
        }
        else{  //p_parent is right child
          if(p_is_left_child){ //p is left child
            bstree_algo::rotate_right_no_parent_fix(p_parent, p);
            p_parent = p;
          }
          bstree_algo::rotate_left(p_grandparent, p_parent, NodeTraits::get_parent(p_grandparent), header);
        }
        NodeTraits::set_color(p_parent, NodeTraits::black());
        break;
      }
    }
    NodeTraits::set_color(NodeTraits::get_parent(header), NodeTraits::black());
  }

};

template<typename NodeTraits>
struct get_algo<RbTreeAlgorithms, NodeTraits>
{
  typedef rbtree_algorithms<NodeTraits> type;
};

template<typename ValueTraits, typename NodePtrCompare, typename ExtraChecker>
struct get_node_checker<RbTreeAlgorithms, ValueTraits, NodePtrCompare, ExtraChecker>
{
  typedef detail::rbtree_node_checker<ValueTraits, NodePtrCompare, ExtraChecker> type;
};

} // namespace intrusive
} // namespace libs
} // namespace intrusive

#endif //AUTODDS_LIBS_INTRUSIVE_RBTREE_ALGORITHMS_HPP_