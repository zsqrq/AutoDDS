//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_BSTREE_ALGORITHMS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_BSTREE_ALGORITHMS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/bstree_algorithms_base.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/intrusive/detail/uncast.hpp"
#include "libs/intrusive/detail/math.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

//! This type is the information that will be filled by insert_unique_check
template <typename NodePtr>
struct insert_commit_data_t
{
  AUTODDS_INTRUSIVE_FORCEINLINE insert_commit_data_t()
      : link_left(false), node()
  {}
  bool     link_left;
  NodePtr  node;
};

template <typename NodePtr>
struct data_for_rebalance_t
{
  NodePtr  x;
  NodePtr  x_parent;
  NodePtr  y;
};

namespace detail {

template<typename ValueTraits, typename NodePtrCompare, typename ExtraChecker>
struct bstree_node_checker : public ExtraChecker
{
  typedef ExtraChecker                            base_checker_t;
  typedef ValueTraits                             value_traits;
  typedef typename value_traits::node_traits      node_traits;
  typedef typename node_traits::const_node_ptr    const_node_ptr;

  struct return_type
      : public base_checker_t::return_type
  {
    AUTODDS_INTRUSIVE_FORCEINLINE return_type()
        : min_key_node_ptr(const_node_ptr()), max_key_node_ptr(const_node_ptr()), node_count(0)
    {}

    const_node_ptr min_key_node_ptr;
    const_node_ptr max_key_node_ptr;
    size_t   node_count;
  };

  AUTODDS_INTRUSIVE_FORCEINLINE bstree_node_checker(const NodePtrCompare& comp, ExtraChecker extra_checker)
      : base_checker_t(extra_checker), comp_(comp)
  {}

  void operator () (const_node_ptr p,
                    const return_type& check_return_left, const return_type& check_return_right,
                    return_type& check_return)
  {
    AUTODDS_ASSERT(!check_return_left.max_key_node_ptr || !comp_(p, check_return_left.max_key_node_ptr));
    AUTODDS_ASSERT(!check_return_right.min_key_node_ptr || !comp_(check_return_right.min_key_node_ptr, p));
    check_return.min_key_node_ptr = node_traits::get_left(p)? check_return_left.min_key_node_ptr : p;
    check_return.max_key_node_ptr = node_traits::get_right(p)? check_return_right.max_key_node_ptr : p;
    check_return.node_count = check_return_left.node_count + check_return_right.node_count + 1;
    base_checker_t::operator()(p, check_return_left, check_return_right, check_return);
  }

  const NodePtrCompare comp_;
};

} // namespace detail


//!   This is an implementation of a binary search tree.
//!   A node in the search tree has references to its children and its parent. This
//!   is to allow traversal of the whole tree from a given node making the
//!   implementation of iterator a pointer to a node.
//!   At the top of the tree a node is used specially. This node's parent pointer
//!   is pointing to the root of the tree. Its left pointer points to the
//!   leftmost node in the tree and the right pointer to the rightmost one.
//!   This node is used to represent the end-iterator.
//!
//!                                            +---------+
//!       header------------------------------>|         |
//!                                            |         |
//!                   +----------(left)--------|         |--------(right)---------+
//!                   |                        +---------+                        |
//!                   |                             |                             |
//!                   |                             | (parent)                    |
//!                   |                             |                             |
//!                   |                             |                             |
//!                   |                        +---------+                        |
//!    root of tree ..|......................> |         |                        |
//!                   |                        |    D    |                        |
//!                   |                        |         |                        |
//!                   |                +-------+---------+-------+                |
//!                   |                |                         |                |
//!                   |                |                         |                |
//!                   |                |                         |                |
//!                   |                |                         |                |
//!                   |                |                         |                |
//!                   |          +---------+                 +---------+          |
//!                   |          |         |                 |         |          |
//!                   |          |    B    |                 |    F    |          |
//!                   |          |         |                 |         |          |
//!                   |       +--+---------+--+           +--+---------+--+       |
//!                   |       |               |           |               |       |
//!                   |       |               |           |               |       |
//!                   |       |               |           |               |       |
//!                   |   +---+-----+   +-----+---+   +---+-----+   +-----+---+   |
//!                   +-->|         |   |         |   |         |   |         |<--+
//!                       |    A    |   |    C    |   |    E    |   |    G    |
//!                       |         |   |         |   |         |   |         |
//!                       +---------+   +---------+   +---------+   +---------+
//!
//! bstree_algorithms is configured with a NodeTraits class, which encapsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the binary search tree
//!
//! <tt>node_ptr</tt>: A pointer to a node
//!
//! <tt>const_node_ptr</tt>: A pointer to a const node
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_parent(const_node_ptr n);</tt>
//!
//! <tt>static void set_parent(node_ptr n, node_ptr parent);</tt>
//!
//! <tt>static node_ptr get_left(const_node_ptr n);</tt>
//!
//! <tt>static void set_left(node_ptr n, node_ptr left);</tt>
//!
//! <tt>static node_ptr get_right(const_node_ptr n);</tt>
//!
//! <tt>static void set_right(node_ptr n, node_ptr right);</tt>
template<typename NodeTraits>
class bstree_algorithms : public bstree_algorithms_base<NodeTraits>
{
 public:
  typedef typename NodeTraits::node              node;
  typedef NodeTraits                             node_traits;
  typedef typename NodeTraits::node_ptr          node_ptr;
  typedef typename NodeTraits::const_node_ptr    const_node_ptr;
  typedef insert_commit_data_t<node_ptr>         insert_commit_data;
  typedef data_for_rebalance_t<node_ptr>         data_for_rebalance;

  typedef bstree_algorithms<NodeTraits>          this_type;
  typedef bstree_algorithms_base<NodeTraits>     base_type;

 private:
  template<typename Disposer>
  struct dispose_subtree_disposer
  {
    AUTODDS_INTRUSIVE_FORCEINLINE dispose_subtree_disposer(Disposer& disposer, node_ptr subtree)
    :   disposer_(disposer), subtree_(subtree)
    {}

    AUTODDS_INTRUSIVE_FORCEINLINE void release()
    {  disposer_ = 0;  }

    AUTODDS_INTRUSIVE_FORCEINLINE ~dispose_subtree_disposer()
    {
      if(disposer_){
        dispose_subtree(subtree_, *disposer_);
      }
    }
    Disposer* disposer_;
    const node_ptr subtree_;
  };
  
 public:
  //! <b>Requires</b>: 'header' is the header node of a tree.
  //! <b>Effects</b>: Returns the first node of the tree, the header if the tree is empty.
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr begin_node(const_node_ptr header) AUTODDS_NOEXCEPT
  {  return node_traits::get_left(header);   }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr end_node(const_node_ptr header) AUTODDS_NOEXCEPT
  {  return detail::uncast(header);   }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr root_node(const_node_ptr header) AUTODDS_NOEXCEPT
  {
    node_ptr p = node_traits::get_parent(header);
    return p ? p : detail::uncast(header);
  }

  //! <b>Requires</b>: 'n' is a node of the tree or a node initialized
  //!   by init(...) or init_node.
  //! <b>Effects</b>: Returns true if the node is initialized by init() or init_node().
  AUTODDS_INTRUSIVE_FORCEINLINE static bool unique(const_node_ptr n) AUTODDS_NOEXCEPT
  { return !NodeTraits::get_parent(n); }

  //! <b>Requires</b>: node1 and node2 can't be header nodes
  //!  of two trees.
  //!
  //! <b>Effects</b>: Swaps two nodes. After the function node1 will be inserted
  //!   in the position node2 before the function. node2 will be inserted in the
  //!   position node1 had before the function.
  static void swap_nodes(node_ptr node1, node_ptr node2) AUTODDS_NOEXCEPT
  {
    if (node1 == node2)
      return;
    node_ptr header1(base_type::get_header(node1)), header2(base_type::get_header(node2));
    swap_nodes(node1, header1, node2, header2);
  }
  
  static void swap_nodes(node_ptr node1, node_ptr header1, node_ptr node2, node_ptr header2) AUTODDS_NOEXCEPT
  {
    if (node1 == node2)
    {
      return;
    }
    if (header1 != header2) {
      if (node1 == NodeTraits::get_left(header1)) {
        NodeTraits::set_left(header1, node2);
      }
      if (node1 == NodeTraits::get_right(header1)) {
        NodeTraits::set_right(header1, node2);
      }
      if(node1 == NodeTraits::get_parent(header1)){
        NodeTraits::set_parent(header1, node2);
      }

      if(node2 == NodeTraits::get_left(header2)){
        NodeTraits::set_left(header2, node1);
      }
      if(node2 == NodeTraits::get_right(header2)){
        NodeTraits::set_right(header2, node1);
      }
      if(node2 == NodeTraits::get_parent(header2)){
        NodeTraits::set_parent(header2, node1);
      }
    } 
    else
    {
      if(node1 == NodeTraits::get_left(header1)){
        NodeTraits::set_left(header1, node2);
      }
      else if(node2 == NodeTraits::get_left(header2)){
        NodeTraits::set_left(header2, node1);
      }

      if(node1 == NodeTraits::get_right(header1)){
        NodeTraits::set_right(header1, node2);
      }
      else if(node2 == NodeTraits::get_right(header2)){
        NodeTraits::set_right(header2, node1);
      }

      if(node1 == NodeTraits::get_parent(header1)){
        NodeTraits::set_parent(header1, node2);
      }
      else if(node2 == NodeTraits::get_parent(header2)){
        NodeTraits::set_parent(header2, node1);
      }

      //Adjust data in nodes to be swapped
      //so that final link swap works as expected
      if(node1 == NodeTraits::get_parent(node2)){
        NodeTraits::set_parent(node2, node2);

        if(node2 == NodeTraits::get_right(node1)){
          NodeTraits::set_right(node1, node1);
        }
        else{
          NodeTraits::set_left(node1, node1);
        }
      }
      else if(node2 == NodeTraits::get_parent(node1)){
        NodeTraits::set_parent(node1, node1);

        if(node1 == NodeTraits::get_right(node2)){
          NodeTraits::set_right(node2, node2);
        }
        else{
          NodeTraits::set_left(node2, node2);
        }
      }
    }
    
    // swap left link
    node_ptr temp;
    temp = NodeTraits::get_left(node1);
    NodeTraits::set_left(node1, NodeTraits::get_left(node2));
    NodeTraits::set_left(node2, temp);
    
    // swap right link
    temp = NodeTraits::get_right(node1);
    NodeTraits::set_right(node1, NodeTraits::get_right(node2));
    NodeTraits::set_right(node2, temp);

    //swap parent link
    temp = NodeTraits::get_parent(node1);
    NodeTraits::set_parent(node1, NodeTraits::get_parent(node2));
    NodeTraits::set_parent(node2, temp);

    //Now adjust child nodes for newly inserted node 1
    if((temp = NodeTraits::get_left(node1))){
      NodeTraits::set_parent(temp, node1);
    }
    if((temp = NodeTraits::get_right(node1))){
      NodeTraits::set_parent(temp, node1);
    }
    
    //Now adjust child nodes for newly inserted node 2
    if((temp = NodeTraits::get_left(node2))){
      NodeTraits::set_parent(temp, node2);
    }
    if((temp = NodeTraits::get_right(node2))){
      NodeTraits::set_parent(temp, node2);
    }

    //Finally adjust parent nodes
    if ((temp = NodeTraits::get_parent(node1)) == NodeTraits::get_parent(node2)) {
      // special logic for the case where the nodes are siblings
      const node_ptr left = NodeTraits::get_left(temp);
      NodeTraits::set_left(temp, NodeTraits::get_right(temp));
      NodeTraits::set_right(temp, left);
    } else {
      if ((temp = NodeTraits::get_parent(node1)) &&
          //The header has been already updated so avoid it
          temp != header2) {
        if (NodeTraits::get_left(temp) == node2) {
          NodeTraits::set_left(temp, node1);
        }
        if (NodeTraits::get_right(temp) == node2) {
          NodeTraits::set_right(temp, node1);
        }
      }
      if ((temp = NodeTraits::get_parent(node2)) &&
          //The header has been already updated so avoid it
          temp != header1) {
        if (NodeTraits::get_left(temp) == node1) {
          NodeTraits::set_left(temp, node2);
        }
        if (NodeTraits::get_right(temp) == node1) {
          NodeTraits::set_right(temp, node2);
        }
      }
    }
  }

  //! <b>Requires</b>: node_to_be_replaced must be inserted in a tree
  //!   and new_node must not be inserted in a tree.
  //!
  //! <b>Effects</b>: Replaces node_to_be_replaced in its position in the
  //!   tree with new_node. The tree does not need to be rebalanced
  AUTODDS_INTRUSIVE_FORCEINLINE static void replace_node(node_ptr node_to_be_replaced,
                                                         node_ptr new_node) AUTODDS_NOEXCEPT
  {
    replace_node(node_to_be_replaced, base_type::get_header(node_to_be_replaced), new_node);
  }

  static void replace_node(node_ptr node_to_be_replaced, node_ptr header, node_ptr new_node) AUTODDS_NOEXCEPT
  {
    AUTODDS_ASSERT(node_to_be_replaced != new_node);

    //Update header if necessary
    if(node_to_be_replaced == NodeTraits::get_left(header)){
      NodeTraits::set_left(header, new_node);
    }

    if(node_to_be_replaced == NodeTraits::get_right(header)){
      NodeTraits::set_right(header, new_node);
    }

    if(node_to_be_replaced == NodeTraits::get_parent(header)){
      NodeTraits::set_parent(header, new_node);
    }

    //Now set data from the original node
    node_ptr temp;
    NodeTraits::set_left(new_node, NodeTraits::get_left(node_to_be_replaced));
    NodeTraits::set_right(new_node, NodeTraits::get_right(node_to_be_replaced));
    NodeTraits::set_parent(new_node, NodeTraits::get_parent(node_to_be_replaced));

    //Now adjust adjacent nodes for newly inserted node
    if((temp = NodeTraits::get_left(new_node))){
      NodeTraits::set_parent(temp, new_node);
    }
    if((temp = NodeTraits::get_right(new_node))){
      NodeTraits::set_parent(temp, new_node);
    }
    if((temp = NodeTraits::get_parent(new_node)) &&
        //The header has been already updated so avoid it
        temp != header){
      if(NodeTraits::get_left(temp) == node_to_be_replaced){
        NodeTraits::set_left(temp, new_node);
      }
      if(NodeTraits::get_right(temp) == node_to_be_replaced){
        NodeTraits::set_right(temp, new_node);
      }
    }
  }

  //! <b>Requires</b>: 'n' must not be part of any tree.
  //! <b>Effects</b>: After the function unique(node) == true.
  static void init(node_ptr n) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_parent(n, node_ptr());
    NodeTraits::set_left(n, node_ptr());
    NodeTraits::set_right(n, node_ptr());
  }

  //! <b>Effects</b>: Returns true if node is in the same state as if called init(node)
  static bool inited(const_node_ptr n)
  {
    return !NodeTraits::get_parent(n) &&
        !NodeTraits::get_left(n)   &&
        !NodeTraits::get_right(n)  ;
  }

  //! <b>Requires</b>: header must not be part of any tree.
  //!
  //! <b>Effects</b>: Initializes the header to represent an empty tree.
  //!   unique(header) == true.
  static void init_header(node_ptr header) AUTODDS_NOEXCEPT
  {
    NodeTraits::set_parent(header, node_ptr());
    NodeTraits::set_left(header, header);
    NodeTraits::set_right(header, header);
  }

  //! <b>Requires</b>: "disposer" must be an object function
  //!   taking a node_ptr parameter and shouldn't throw.
  //!
  //! <b>Effects</b>: Empties the target tree calling
  //!   <tt>void disposer::operator()(node_ptr)</tt> for every node of the tree
  //!    except the header.
  template<typename Disposer>
  static void clear_and_dispose(node_ptr header, Disposer disposer) AUTODDS_NOEXCEPT
  {
    node_ptr source_root = NodeTraits::get_parent(header);
    if(!source_root)
      return;
    dispose_subtree(source_root, disposer);
    init_header(header);
  }

  //! <b>Requires</b>: header is the header of a tree.
  //!
  //! <b>Effects</b>: Unlinks the leftmost node from the tree, and
  //!   updates the header link to the new leftmost node.
  static node_ptr unlink_leftmost_without_rebalance(node_ptr header) AUTODDS_NOEXCEPT
  {
    node_ptr leftmost = NodeTraits::get_left(header);
    if (leftmost == header)
      return node_ptr();
    node_ptr leftmost_parent(NodeTraits::get_parent(leftmost));
    node_ptr leftmost_right (NodeTraits::get_right(leftmost));
    bool is_root = leftmost_parent == header;

    if (leftmost_right){
      NodeTraits::set_parent(leftmost_right, leftmost_parent);
      NodeTraits::set_left(header, base_type::minimum(leftmost_right));

      if (is_root)
        NodeTraits::set_parent(header, leftmost_right);
      else
        NodeTraits::set_left(NodeTraits::get_parent(header), leftmost_right);
    }
    else if (is_root){
      NodeTraits::set_parent(header, node_ptr());
      NodeTraits::set_left(header,  header);
      NodeTraits::set_right(header, header);
    }
    else{
      NodeTraits::set_left(leftmost_parent, node_ptr());
      NodeTraits::set_left(header, leftmost_parent);
    }
    return leftmost;
  }

  //! <b>Effects</b>: Returns the number of nodes of the tree.
  static std::size_t size(const_node_ptr header) AUTODDS_NOEXCEPT
  {
    node_ptr beg(begin_node(header));
    node_ptr end(end_node(header));
    std::size_t i = 0;
    for(;beg != end; beg = base_type::next_node(beg)) ++i;
    return i;
  }

  //! <b>Effects</b>: Swaps two trees. After the function header1 will contain
  //!   links to the second tree and header2 will have links to the first tree.
  static void swap_tree(node_ptr header1, node_ptr header2) AUTODDS_NOEXCEPT
  {
    if(header1 == header2)
      return;

    node_ptr tmp;

    //Parent swap
    tmp = NodeTraits::get_parent(header1);
    NodeTraits::set_parent(header1, NodeTraits::get_parent(header2));
    NodeTraits::set_parent(header2, tmp);
    //Left swap
    tmp = NodeTraits::get_left(header1);
    NodeTraits::set_left(header1, NodeTraits::get_left(header2));
    NodeTraits::set_left(header2, tmp);
    //Right swap
    tmp = NodeTraits::get_right(header1);
    NodeTraits::set_right(header1, NodeTraits::get_right(header2));
    NodeTraits::set_right(header2, tmp);

    //Now test parent
    node_ptr h1_parent(NodeTraits::get_parent(header1));
    if(h1_parent){
      NodeTraits::set_parent(h1_parent, header1);
    }
    else{
      NodeTraits::set_left(header1, header1);
      NodeTraits::set_right(header1, header1);
    }

    node_ptr h2_parent(NodeTraits::get_parent(header2));
    if(h2_parent){
      NodeTraits::set_parent(h2_parent, header2);
    }
    else{
      NodeTraits::set_left(header2, header2);
      NodeTraits::set_right(header2, header2);
    }
  }

  //! <b>Effects</b>: Returns a node_ptr to the first element that is equivalent to
  //!   "key" according to "comp" or "header" if that element does not exist.
  template<typename KeyType, typename KeyNodePtrCompare>
  static node_ptr find
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    node_ptr end = detail::uncast(header);
    node_ptr y = lower_bound(header, key, comp);
    return (y == end || comp(key, y)) ? end : y;
  }

  //! <b>Effects</b>: Returns an a pair with the following criteria:
  template< typename KeyType, typename KeyNodePtrCompare>
  static std::pair<node_ptr, node_ptr> bounded_range
      ( const_node_ptr header
          , const KeyType &lower_key
          , const KeyType &upper_key
          , KeyNodePtrCompare comp
          , bool left_closed
          , bool right_closed)
  {
    node_ptr y = detail::uncast(header);
    node_ptr x = NodeTraits::get_parent(header);

    while(x){
      //If x is less than lower_key the target
      //range is on the right part
      if(comp(x, lower_key)){
        //Check for invalid input range
        AUTODDS_INTRUSIVE_INVARIANT_ASSERT(comp(x, upper_key));
        x = NodeTraits::get_right(x);
      }
        //If the upper_key is less than x, the target
        //range is on the left part
      else if(comp(upper_key, x)){
        y = x;
        x = NodeTraits::get_left(x);
      }
      else{
        //x is inside the bounded range(lower_key <= x <= upper_key),
        //so we must split lower and upper searches
        //
        //Sanity check: if lower_key and upper_key are equal, then both left_closed and right_closed can't be false
        AUTODDS_INTRUSIVE_INVARIANT_ASSERT(left_closed || right_closed || comp(lower_key, x) || comp(x, upper_key));
        return std::pair<node_ptr,node_ptr>(
            left_closed ? lower_bound_loop(NodeTraits::get_left(x), x, lower_key, comp): 
            upper_bound_loop(x, y, lower_key, comp),
            right_closed ? 
            upper_bound_loop(NodeTraits::get_right(x), y, upper_key, comp): 
            lower_bound_loop(x, y, upper_key, comp)
        );
      }
    }
    return std::pair<node_ptr,node_ptr> (y, y);
  }

  //! <b>Effects</b>: Returns the number of elements with a key equivalent to "key"
  template<typename KeyType, typename KeyNodePtrCompare>
  static std::size_t count
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    std::pair<node_ptr, node_ptr> ret = equal_range(header, key, comp);
    std::size_t n = 0;
    while(ret.first != ret.second){
      ++n;
      ret.first = base_type::next_node(ret.first);
    }
    return n;
  }

  //! <b>Effects</b>: Returns an a pair of node_ptr delimiting a range containing
  //!   all elements that are equivalent to "key" according to "comp" or an
  //!   empty range that indicates the position where those elements would be
  //!   if there are no equivalent elements.
  template<typename KeyType, typename KeyNodePtrCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE static std::pair<node_ptr, node_ptr> equal_range
  (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    return bounded_range(header, key, key, comp, true, true);
  }

  //! <b>Effects</b>: Returns an a pair of node_ptr delimiting a range containing
  //!   the first element that is equivalent to "key" according to "comp" or an
  //!   empty range that indicates the position where that element would be
  //!   if there are no equivalent elements.
  template<typename KeyType, typename KeyNodePtrCompare>
  static std::pair<node_ptr, node_ptr> lower_bound_range
      (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    node_ptr const lb(lower_bound(header, key, comp));
    std::pair<node_ptr, node_ptr> ret_ii(lb, lb);
    if(lb != header && !comp(key, lb)){
      ret_ii.second = base_type::next_node(ret_ii.second);
    }
    return ret_ii;
  }

  //! <b>Effects</b>: Returns a node_ptr to the first element that is
  //!   not less than "key" according to "comp" or "header" if that element does
  //!   not exist.
  template<typename KeyType, typename KeyNodePtrCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr lower_bound
  (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    return lower_bound_loop(NodeTraits::get_parent(header), detail::uncast(header), key, comp);
  }

  //! <b>Effects</b>: Returns a node_ptr to the first element that is greater
  //!   than "key" according to "comp" or "header" if that element does not exist.
  template<typename KeyType, typename KeyNodePtrCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr upper_bound
  (const_node_ptr header, const KeyType &key, KeyNodePtrCompare comp)
  {
    return upper_bound_loop(NodeTraits::get_parent(header), detail::uncast(header), key, comp);
  }

  //! <b>Effects</b>: Inserts new_node in the set using the information obtained
  //!   from the "commit_data" that a previous "insert_check" filled.
  AUTODDS_INTRUSIVE_FORCEINLINE static void insert_unique_commit
      (node_ptr header, node_ptr new_value, const insert_commit_data &commit_data) AUTODDS_NOEXCEPT
  {  return insert_commit(header, new_value, commit_data); }

  //! <b>Effects</b>: Checks if there is an equivalent node to "key" in the
  //!   tree according to "comp" and obtains the needed information to realize
  //!   a constant-time node insertion if there is no equivalent node.
  template<typename KeyType, typename KeyNodePtrCompare>
  static std::pair<node_ptr, bool> insert_unique_check
      (const_node_ptr header, const KeyType &key
          ,KeyNodePtrCompare comp, insert_commit_data &commit_data, std::size_t *pdepth = 0)
  {
    std::size_t depth = 0;
    node_ptr h(detail::uncast(header));
    node_ptr y(h);
    node_ptr x(NodeTraits::get_parent(y));
    node_ptr prev = node_ptr();

    //Find the upper bound, cache the previous value and if we should
    //store it in the left or right node
    bool left_child = true;
    while(x){
      ++depth;
      y = x;
      left_child = comp(key, x);
      x = left_child ?
          NodeTraits::get_left(x) : (prev = y, NodeTraits::get_right(x));
    }

    if(pdepth)  *pdepth = depth;

    //Since we've found the upper bound there is no other value with the same key if:
    //    - There is no previous node
    //    - The previous node is less than the key
    const bool not_present = !prev || comp(prev, key);
    if(not_present){
      commit_data.link_left = left_child;
      commit_data.node      = y;
    }
    return std::pair<node_ptr, bool>(prev, not_present);
  }

  //! <b>Effects</b>: Checks if there is an equivalent node to "key" in the
  //!   tree according to "comp" using "hint" as a hint to where it should be
  //!   inserted and obtains the needed information to realize
  //!   a constant-time node insertion if there is no equivalent node.
  //!   If "hint" is the upper_bound the function has constant time
  //!   complexity (two comparisons in the worst case).
  template<typename KeyType, typename KeyNodePtrCompare>
  static std::pair<node_ptr, bool> insert_unique_check
      (const_node_ptr header, node_ptr hint, const KeyType &key
          ,KeyNodePtrCompare comp, insert_commit_data &commit_data, std::size_t *pdepth = 0)
  {
    //hint must be bigger than the key
    if(hint == header || comp(key, hint)){
      node_ptr prev(hint);
      //Previous value should be less than the key
      if(hint == begin_node(header) || comp((prev = base_type::prev_node(hint)), key)){
        commit_data.link_left = unique(header) || !NodeTraits::get_left(hint);
        commit_data.node      = commit_data.link_left ? hint : prev;
        if(pdepth){
          *pdepth = commit_data.node == header ? 0 : depth(commit_data.node) + 1;
        }
        return std::pair<node_ptr, bool>(node_ptr(), true);
      }
    }
    //Hint was wrong, use hintless insertion
    return insert_unique_check(header, key, comp, commit_data, pdepth);
  }

  //! <b>Effects</b>: Inserts new_node into the tree, using "hint" as a hint to
  //!   where it will be inserted. If "hint" is the upper_bound
  //!   the insertion takes constant time (two comparisons in the worst case).
  template<typename NodePtrCompare>
  static node_ptr insert_equal
      (node_ptr h, node_ptr hint, node_ptr new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
  {
    insert_commit_data commit_data;
    insert_equal_check(h, hint, new_node, comp, commit_data, pdepth);
    insert_commit(h, new_node, commit_data);
    return new_node;
  }

  //! <b>Effects</b>: Inserts new_node into the tree before the upper bound
  //!   according to "comp".
  template<typename NodePtrCompare>
  static node_ptr insert_equal_upper_bound
      (node_ptr h, node_ptr new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
  {
    insert_commit_data commit_data;
    insert_equal_upper_bound_check(h, new_node, comp, commit_data, pdepth);
    insert_commit(h, new_node, commit_data);
    return new_node;
  }

  //! <b>Effects</b>: Inserts new_node into the tree before the lower bound
  //!   according to "comp".
  template<class NodePtrCompare>
  static node_ptr insert_equal_lower_bound
      (node_ptr h, node_ptr new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
  {
    insert_commit_data commit_data;
    insert_equal_lower_bound_check(h, new_node, comp, commit_data, pdepth);
    insert_commit(h, new_node, commit_data);
    return new_node;
  }

  //! <b>Effects</b>: Inserts new_node into the tree before "pos".
  static node_ptr insert_before
      (node_ptr header, node_ptr pos, node_ptr new_node, std::size_t *pdepth = 0) AUTODDS_NOEXCEPT
  {
    insert_commit_data commit_data;
    insert_before_check(header, pos, commit_data, pdepth);
    insert_commit(header, new_node, commit_data);
    return new_node;
  }

  //! <b>Effects</b>: Inserts new_node into the tree before "pos".
  static void push_back(node_ptr header, node_ptr new_node, std::size_t *pdepth = 0) AUTODDS_NOEXCEPT
  {
    insert_commit_data commit_data;
    push_back_check(header, commit_data, pdepth);
    insert_commit(header, new_node, commit_data);
  }

  //! <b>Effects</b>: Inserts new_node into the tree before "pos".
  static void push_front(node_ptr header, node_ptr new_node, std::size_t *pdepth = 0) AUTODDS_NOEXCEPT
  {
    insert_commit_data commit_data;
    push_front_check(header, commit_data, pdepth);
    insert_commit(header, new_node, commit_data);
  }

  //! <b>Effects</b>: Calculates the depth of a node: the depth of a
  //! node is the length (number of edges) of the path from the root
  //! to that node. (The root node is at depth 0.)
  static std::size_t depth(const_node_ptr n) AUTODDS_NOEXCEPT
  {
    std::size_t depth = 0;
    node_ptr p_parent;
    while(n != NodeTraits::get_parent(p_parent = NodeTraits::get_parent(n))){
      ++depth;
      n = p_parent;
    }
    return depth;
  }

  //! <b>Effects</b>: First empties target tree calling
  //!   <tt>void disposer::operator()(node_ptr)</tt> for every node of the tree
  //!    except the header.
  template <typename Cloner, typename Disposer>
  static void clone
      (const_node_ptr source_header, node_ptr target_header, Cloner cloner, Disposer disposer)
  {
    if(!unique(target_header)){
      clear_and_dispose(target_header, disposer);
    }

    node_ptr leftmost, rightmost;
    node_ptr new_root = clone_subtree
        (source_header, target_header, cloner, disposer, leftmost, rightmost);

    //Now update header node
    NodeTraits::set_parent(target_header, new_root);
    NodeTraits::set_left  (target_header, leftmost);
    NodeTraits::set_right (target_header, rightmost);
  }

  //! <b>Effects</b>: Erases node "z" from the tree with header "header".
  AUTODDS_INTRUSIVE_FORCEINLINE static void erase(node_ptr header, node_ptr z) AUTODDS_NOEXCEPT
  {
    data_for_rebalance ignored;
    erase(header, z, ignored);
  }

  //! <b>Effects</b>: Transfers node "z" from tree1 to tree2 if tree1 does not contain
  //!   a node that is equivalent to z.
  template<typename NodePtrCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE static bool transfer_unique
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z)
  {
    data_for_rebalance ignored;
    return transfer_unique(header1, comp, header2, z, ignored);
  }

  //! <b>Effects</b>: Transfers node "z" from tree1 to tree2.
  template<class NodePtrCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE static void transfer_equal
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z)
  {
    data_for_rebalance ignored;
    transfer_equal(header1, comp, header2, z, ignored);
  }

  //! <b>Effects</b>: Unlinks the node and rebalances the tree.
  static void unlink(node_ptr n) AUTODDS_NOEXCEPT
  {
    node_ptr x = NodeTraits::get_parent(n);
    if(x){
      while(!base_type::is_header(x))
        x = NodeTraits::get_parent(x);
      erase(x, n);
    }
  }

  //! <b>Effects</b>: Rebalances the tree.
  static void rebalance(node_ptr header) AUTODDS_NOEXCEPT
  {
    node_ptr root = NodeTraits::get_parent(header);
    if(root){
      rebalance_subtree(root);
    }
  }

  //! <b>Effects</b>: Rebalances the subtree rooted at old_root.
  static node_ptr rebalance_subtree(node_ptr old_root) AUTODDS_NOEXCEPT
  {
    //Taken from:
    //"Tree rebalancing in optimal time and space"
    //Quentin F. Stout and Bette L. Warren
    node_ptr super_root = NodeTraits::get_parent(old_root);
    AUTODDS_INTRUSIVE_INVARIANT_ASSERT(super_root);

    //Get root info
    node_ptr super_root_right_backup = NodeTraits::get_right(super_root);
    bool super_root_is_header = NodeTraits::get_parent(super_root) == old_root;
    bool old_root_is_right  = is_right_child(old_root);
    NodeTraits::set_right(super_root, old_root);

    std::size_t size;
    subtree_to_vine(super_root, size);
    vine_to_subtree(super_root, size);
    node_ptr new_root = NodeTraits::get_right(super_root);

    //Recover root
    if(super_root_is_header){
      NodeTraits::set_right(super_root, super_root_right_backup);
      NodeTraits::set_parent(super_root, new_root);
    }
    else if(old_root_is_right){
      NodeTraits::set_right(super_root, new_root);
    }
    else{
      NodeTraits::set_right(super_root, super_root_right_backup);
      NodeTraits::set_left(super_root, new_root);
    }
    return new_root;
  }

  //! <b>Effects</b>: Asserts the integrity of the container with additional checks provided by the user.
  template<typename Checker>
  static void check(const_node_ptr header, Checker checker, typename Checker::return_type& checker_return)
  {
    const_node_ptr root_node_ptr = NodeTraits::get_parent(header);
    if (!root_node_ptr){
      // check left&right header pointers
      AUTODDS_ASSERT(NodeTraits::get_left(header) == header);
      AUTODDS_ASSERT(NodeTraits::get_right(header) == header);
    }
    else{
      // check parent pointer of root node
      AUTODDS_ASSERT(NodeTraits::get_parent(root_node_ptr) == header);
      // check subtree from root
      check_subtree(root_node_ptr, checker, checker_return);
      // check left&right header pointers
      const_node_ptr p = root_node_ptr;
      while (NodeTraits::get_left(p)) { p = NodeTraits::get_left(p); }
      AUTODDS_ASSERT(NodeTraits::get_left(header) == p);
      p = root_node_ptr;
      while (NodeTraits::get_right(p)) { p = NodeTraits::get_right(p); }
      AUTODDS_ASSERT(NodeTraits::get_right(header) == p);
    }
  }
  
 protected:

  template<typename NodePtrCompare>
  static bool transfer_unique
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z, data_for_rebalance &info)
  {
    insert_commit_data commit_data;
    bool const transferable = insert_unique_check(header1, z, comp, commit_data).second;
    if(transferable){
      erase(header2, z, info);
      insert_commit(header1, z, commit_data);
    }
    return transferable;
  }

  template<typename NodePtrCompare>
  static void transfer_equal
      (node_ptr header1, NodePtrCompare comp, node_ptr header2, node_ptr z, data_for_rebalance &info)
  {
    insert_commit_data commit_data;
    insert_equal_upper_bound_check(header1, z, comp, commit_data);
    erase(header2, z, info);
    insert_commit(header1, z, commit_data);
  }

  static void erase(node_ptr header, node_ptr z, data_for_rebalance &info)
  {
    node_ptr y(z);
    node_ptr x;
    const node_ptr z_left(NodeTraits::get_left(z));
    const node_ptr z_right(NodeTraits::get_right(z));

    if(!z_left){
      x = z_right;    // x might be null.
    }
    else if(!z_right){ // z has exactly one non-null child. y == z.
      x = z_left;       // x is not null.
      AUTODDS_ASSERT(x);
    }
    else{ //make y != z
      // y = find z's successor
      y = base_type::minimum(z_right);
      x = NodeTraits::get_right(y);     // x might be null.
    }

    node_ptr x_parent;
    const node_ptr z_parent(NodeTraits::get_parent(z));
    const bool z_is_leftchild(NodeTraits::get_left(z_parent) == z);

    if(y != z){ //has two children and y is the minimum of z
      //y is z's successor and it has a null left child.
      //x is the right child of y (it can be null)
      //Relink y in place of z and link x with y's old parent
      NodeTraits::set_parent(z_left, y);
      NodeTraits::set_left(y, z_left);
      if(y != z_right){
        //Link y with the right tree of z
        NodeTraits::set_right(y, z_right);
        NodeTraits::set_parent(z_right, y);
        //Link x with y's old parent (y must be a left child)
        x_parent = NodeTraits::get_parent(y);
        AUTODDS_ASSERT(NodeTraits::get_left(x_parent) == y);
        if(x)
          NodeTraits::set_parent(x, x_parent);
        //Since y was the successor and not the right child of z, it must be a left child
        NodeTraits::set_left(x_parent, x);
      }
      else{ //y was the right child of y so no need to fix x's position
        x_parent = y;
      }
      NodeTraits::set_parent(y, z_parent);
      this_type::set_child(header, y, z_parent, z_is_leftchild);
    }
    else {  // z has zero or one child, x is one child (it can be null)
      //Just link x to z's parent
      x_parent = z_parent;
      if(x)
        NodeTraits::set_parent(x, z_parent);
      this_type::set_child(header, x, z_parent, z_is_leftchild);

      //Now update leftmost/rightmost in case z was one of them
      if(NodeTraits::get_left(header) == z){
        //z_left must be null because z is the leftmost
        AUTODDS_ASSERT(!z_left);
        NodeTraits::set_left(header, !z_right ?
                                     z_parent :  // makes leftmost == header if z == root
                                     base_type::minimum(z_right));
      }
      if(NodeTraits::get_right(header) == z){
        //z_right must be null because z is the rightmost
        AUTODDS_ASSERT(!z_right);
        NodeTraits::set_right(header, !z_left ?
                                      z_parent :  // makes rightmost == header if z == root
                                      base_type::maximum(z_left));
      }
    }

    //If z had 0/1 child, y == z and one of its children (and maybe null)
    //If z had 2 children, y is the successor of z and x is the right child of y
    info.x = x;
    info.y = y;
    //If z had 0/1 child, x_parent is the new parent of the old right child of y (z's successor)
    //If z had 2 children, x_parent is the new parent of y (z_parent)
    AUTODDS_ASSERT(!x || NodeTraits::get_parent(x) == x_parent);
    info.x_parent = x_parent;
  }

  //! <b>Effects</b>: Returns the number of nodes of the subtree.
  static std::size_t subtree_size(const_node_ptr subtree) AUTODDS_NOEXCEPT
  {
    std::size_t count = 0;
    if (subtree){
      node_ptr n = detail::uncast(subtree);
      node_ptr m = NodeTraits::get_left(n);
      while(m){
        n = m;
        m = NodeTraits::get_left(n);
      }

      while(1){
        ++count;
        node_ptr n_right(NodeTraits::get_right(n));
        if(n_right){
          n = n_right;
          m = NodeTraits::get_left(n);
          while(m){
            n = m;
            m = NodeTraits::get_left(n);
          }
        }
        else {
          do{
            if (n == subtree){
              return count;
            }
            m = n;
            n = NodeTraits::get_parent(n);
          }while(NodeTraits::get_left(n) != m);
        }
      }
    }
    return count;
  }
  
  //! <b>Effects</b>: Returns true if p is a left child.
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_left_child(node_ptr p) AUTODDS_NOEXCEPT
  {  return NodeTraits::get_left(NodeTraits::get_parent(p)) == p;  }

  //! <b>Effects</b>: Returns true if p is a right child.
  AUTODDS_INTRUSIVE_FORCEINLINE static bool is_right_child(node_ptr p) AUTODDS_NOEXCEPT
  {  return NodeTraits::get_right(NodeTraits::get_parent(p)) == p;  }

  static void insert_before_check
      (node_ptr header, node_ptr pos, insert_commit_data &commit_data, std::size_t *pdepth = 0)
  {
    node_ptr prev(pos);
    if(pos != NodeTraits::get_left(header))
      prev = base_type::prev_node(pos);
    bool link_left = unique(header) || !NodeTraits::get_left(pos);
    commit_data.link_left = link_left;
    commit_data.node = link_left ? pos : prev;
    if(pdepth){
      *pdepth = commit_data.node == header ? 0 : depth(commit_data.node) + 1;
    }
  }

  static void push_back_check
      (node_ptr header, insert_commit_data &commit_data, std::size_t *pdepth = 0) AUTODDS_NOEXCEPT
  {
    node_ptr prev(NodeTraits::get_right(header));
    if(pdepth){
      *pdepth = prev == header ? 0 : depth(prev) + 1;
    }
    commit_data.link_left = false;
    commit_data.node = prev;
  }

  static void push_front_check
      (node_ptr header, insert_commit_data &commit_data, std::size_t *pdepth = 0) AUTODDS_NOEXCEPT
  {
    node_ptr pos(NodeTraits::get_left(header));
    if(pdepth){
      *pdepth = pos == header ? 0 : depth(pos) + 1;
    }
    commit_data.link_left = true;
    commit_data.node = pos;
  }

  template<typename NodePtrCompare>
  static void insert_equal_check
      (node_ptr header, node_ptr hint, node_ptr new_node, NodePtrCompare comp, 
       insert_commit_data &commit_data, std::size_t *pdepth = 0)
  {
    if(hint == header || !comp(hint, new_node)){
      node_ptr prev(hint);
      if(hint == NodeTraits::get_left(header) ||
          !comp(new_node, (prev = base_type::prev_node(hint)))){
        bool link_left = unique(header) || !NodeTraits::get_left(hint);
        commit_data.link_left = link_left;
        commit_data.node = link_left ? hint : prev;
        if(pdepth){
          *pdepth = commit_data.node == header ? 0 : depth(commit_data.node) + 1;
        }
      }
      else{
        insert_equal_upper_bound_check(header, new_node, comp, commit_data, pdepth);
      }
    }
    else{
      insert_equal_lower_bound_check(header, new_node, comp, commit_data, pdepth);
    }
  }

  template<typename NodePtrCompare>
  static void insert_equal_upper_bound_check
      (node_ptr h, node_ptr new_node, NodePtrCompare comp, insert_commit_data & commit_data, std::size_t *pdepth = 0)
  {
    std::size_t depth = 0;
    node_ptr y(h);
    node_ptr x(NodeTraits::get_parent(y));

    while(x){
      ++depth;
      y = x;
      x = comp(new_node, x) ?
          NodeTraits::get_left(x) : NodeTraits::get_right(x);
    }
    if(pdepth)  *pdepth = depth;
    commit_data.link_left = (y == h) || comp(new_node, y);
    commit_data.node = y;
  }

  template<typename NodePtrCompare>
  static void insert_equal_lower_bound_check
      (node_ptr h, node_ptr new_node, NodePtrCompare comp, insert_commit_data & commit_data, std::size_t *pdepth = 0)
  {
    std::size_t depth = 0;
    node_ptr y(h);
    node_ptr x(NodeTraits::get_parent(y));

    while(x){
      ++depth;
      y = x;
      x = !comp(x, new_node) ?
          NodeTraits::get_left(x) : NodeTraits::get_right(x);
    }
    if(pdepth)  *pdepth = depth;
    commit_data.link_left = (y == h) || !comp(y, new_node);
    commit_data.node = y;
  }

  static void insert_commit
      (node_ptr header, node_ptr new_node, const insert_commit_data &commit_data) AUTODDS_NOEXCEPT
  {
    //Check if commit_data has not been initialized by a insert_unique_check call.
    AUTODDS_ASSERT(commit_data.node != node_ptr());
    node_ptr parent_node(commit_data.node);
    if(parent_node == header){
      NodeTraits::set_parent(header, new_node);
      NodeTraits::set_right(header, new_node);
      NodeTraits::set_left(header, new_node);
    }
    else if(commit_data.link_left){
      NodeTraits::set_left(parent_node, new_node);
      if(parent_node == NodeTraits::get_left(header))
        NodeTraits::set_left(header, new_node);
    }
    else{
      NodeTraits::set_right(parent_node, new_node);
      if(parent_node == NodeTraits::get_right(header))
        NodeTraits::set_right(header, new_node);
    }
    NodeTraits::set_parent(new_node, parent_node);
    NodeTraits::set_right(new_node, node_ptr());
    NodeTraits::set_left(new_node, node_ptr());
  }

  //Fix header and own's parent data when replacing x with own, providing own's old data with parent
  static void set_child(node_ptr header, node_ptr new_child, node_ptr new_parent, const bool link_left) AUTODDS_NOEXCEPT
  {
    if(new_parent == header)
      NodeTraits::set_parent(header, new_child);
    else if(link_left)
      NodeTraits::set_left(new_parent, new_child);
    else
      NodeTraits::set_right(new_parent, new_child);
  }

  // rotate p to left (no header and p's parent fixup)
  static void rotate_left_no_parent_fix(node_ptr p, node_ptr p_right) AUTODDS_NOEXCEPT
  {
    node_ptr p_right_left(NodeTraits::get_left(p_right));
    NodeTraits::set_right(p, p_right_left);
    if(p_right_left){
      NodeTraits::set_parent(p_right_left, p);
    }
    NodeTraits::set_left(p_right, p);
    NodeTraits::set_parent(p, p_right);
  }

  // rotate p to left (with header and p's parent fixup)
  static void rotate_left(node_ptr p, node_ptr p_right, node_ptr p_parent, node_ptr header) AUTODDS_NOEXCEPT
  {
    const bool p_was_left(NodeTraits::get_left(p_parent) == p);
    rotate_left_no_parent_fix(p, p_right);
    NodeTraits::set_parent(p_right, p_parent);
    set_child(header, p_right, p_parent, p_was_left);
  }

  // rotate p to right (no header and p's parent fixup)
  static void rotate_right_no_parent_fix(node_ptr p, node_ptr p_left) AUTODDS_NOEXCEPT
  {
    node_ptr p_left_right(NodeTraits::get_right(p_left));
    NodeTraits::set_left(p, p_left_right);
    if(p_left_right){
      NodeTraits::set_parent(p_left_right, p);
    }
    NodeTraits::set_right(p_left, p);
    NodeTraits::set_parent(p, p_left);
  }

  // rotate p to right (with header and p's parent fixup)
  static void rotate_right(node_ptr p, node_ptr p_left, node_ptr p_parent, node_ptr header) AUTODDS_NOEXCEPT
  {
    const bool p_was_left(NodeTraits::get_left(p_parent) == p);
    rotate_right_no_parent_fix(p, p_left);
    NodeTraits::set_parent(p_left, p_parent);
    set_child(header, p_left, p_parent, p_was_left);
  }

 private:

  static void subtree_to_vine(node_ptr vine_tail, std::size_t &size) AUTODDS_NOEXCEPT
  {
    //Inspired by LibAVL:
    std::size_t len = 0;
    node_ptr remainder = NodeTraits::get_right(vine_tail);
    while(remainder){
      node_ptr tempptr = NodeTraits::get_left(remainder);
      if(!tempptr){   //move vine-tail down one
        vine_tail = remainder;
        remainder = NodeTraits::get_right(remainder);
        ++len;
      }
      else{ //rotate
        NodeTraits::set_left(remainder, NodeTraits::get_right(tempptr));
        NodeTraits::set_right(tempptr, remainder);
        remainder = tempptr;
        NodeTraits::set_right(vine_tail, tempptr);
      }
    }
    size = len;
  }

  static void compress_subtree(node_ptr scanner, std::size_t count) AUTODDS_NOEXCEPT
  {
    while(count--){   //compress "count" spine nodes in the tree with pseudo-root scanner
      node_ptr child = NodeTraits::get_right(scanner);
      node_ptr child_right = NodeTraits::get_right(child);
      NodeTraits::set_right(scanner, child_right);
      //Avoid setting the parent of child_right
      scanner = child_right;
      node_ptr scanner_left = NodeTraits::get_left(scanner);
      NodeTraits::set_right(child, scanner_left);
      if(scanner_left)
        NodeTraits::set_parent(scanner_left, child);
      NodeTraits::set_left(scanner, child);
      NodeTraits::set_parent(child, scanner);
    }
  }

  static void vine_to_subtree(node_ptr super_root, std::size_t count) AUTODDS_NOEXCEPT
  {
    const std::size_t one_szt = 1u;
    std::size_t leaf_nodes = count + one_szt - std::size_t(one_szt << detail::floor_log2(count + one_szt));
    compress_subtree(super_root, leaf_nodes);  //create deepest leaves
    std::size_t vine_nodes = count - leaf_nodes;
    while(vine_nodes > 1){
      vine_nodes /= 2;
      compress_subtree(super_root, vine_nodes);
    }

    //Update parents of nodes still in the in the original vine line
    //as those have not been updated by subtree_to_vine or compress_subtree
    for ( node_ptr q = super_root, p = NodeTraits::get_right(super_root)
        ; p
        ; q = p, p = NodeTraits::get_right(p)){
      NodeTraits::set_parent(p, q);
    }
  }

  //! <b>Effects</b>: Returns a pointer to the header node of the tree.
  static node_ptr get_root(node_ptr n) AUTODDS_NOEXCEPT
  {
    AUTODDS_INTRUSIVE_INVARIANT_ASSERT((!inited(n)));
    node_ptr x = NodeTraits::get_parent(n);
    if(x){
      while(!base_type::is_header(x)){
        x = NodeTraits::get_parent(x);
      }
      return x;
    }
    else{
      return n;
    }
  }

  template <typename Cloner, typename Disposer>
  static node_ptr clone_subtree
      (const_node_ptr source_parent, node_ptr target_parent, Cloner cloner, 
       Disposer disposer, node_ptr &leftmost_out, node_ptr &rightmost_out)
  {
    node_ptr target_sub_root = target_parent;
    node_ptr source_root = NodeTraits::get_parent(source_parent);
    if(!source_root){
      leftmost_out = rightmost_out = source_root;
    }
    else{
      //We'll calculate leftmost and rightmost nodes while iterating
      node_ptr current = source_root;
      node_ptr insertion_point = target_sub_root = cloner(current);

      //We'll calculate leftmost and rightmost nodes while iterating
      node_ptr leftmost  = target_sub_root;
      node_ptr rightmost = target_sub_root;

      //First set the subroot
      NodeTraits::set_left(target_sub_root, node_ptr());
      NodeTraits::set_right(target_sub_root, node_ptr());
      NodeTraits::set_parent(target_sub_root, target_parent);

      dispose_subtree_disposer<Disposer> rollback(disposer, target_sub_root);
      while(true) {
        //First clone left nodes
        if( NodeTraits::get_left(current) &&
            !NodeTraits::get_left(insertion_point)) {
          current = NodeTraits::get_left(current);
          node_ptr temp = insertion_point;
          //Clone and mark as leaf
          insertion_point = cloner(current);
          NodeTraits::set_left  (insertion_point, node_ptr());
          NodeTraits::set_right (insertion_point, node_ptr());
          //Insert left
          NodeTraits::set_parent(insertion_point, temp);
          NodeTraits::set_left  (temp, insertion_point);
          //Update leftmost
          if(rightmost == target_sub_root)
            leftmost = insertion_point;
        }
          //Then clone right nodes
        else if( NodeTraits::get_right(current) &&
            !NodeTraits::get_right(insertion_point)){
          current = NodeTraits::get_right(current);
          node_ptr temp = insertion_point;
          //Clone and mark as leaf
          insertion_point = cloner(current);
          NodeTraits::set_left  (insertion_point, node_ptr());
          NodeTraits::set_right (insertion_point, node_ptr());
          //Insert right
          NodeTraits::set_parent(insertion_point, temp);
          NodeTraits::set_right (temp, insertion_point);
          //Update rightmost
          rightmost = insertion_point;
        }
          //If not, go up
        else if(current == source_root){
          break;
        }
        else{
          //Branch completed, go up searching more nodes to clone
          current = NodeTraits::get_parent(current);
          insertion_point = NodeTraits::get_parent(insertion_point);
        }
      }
      rollback.release();
      leftmost_out   = leftmost;
      rightmost_out  = rightmost;
    }
    return target_sub_root;
  }

  template<typename Disposer>
  static void dispose_subtree(node_ptr x, Disposer disposer) AUTODDS_NOEXCEPT
  {
    while (x){
      node_ptr save(NodeTraits::get_left(x));
      if (save) {
        // Right rotation
        NodeTraits::set_left(x, NodeTraits::get_right(save));
        NodeTraits::set_right(save, x);
      }
      else {
        save = NodeTraits::get_right(x);
        init(x);
        disposer(x);
      }
      x = save;
    }
  }

  template<typename KeyType, typename KeyNodePtrCompare>
  static node_ptr lower_bound_loop
      (node_ptr x, node_ptr y, const KeyType &key, KeyNodePtrCompare comp)
  {
    while(x){
      if(comp(x, key)){
        x = NodeTraits::get_right(x);
      }
      else{
        y = x;
        x = NodeTraits::get_left(x);
      }
    }
    return y;
  }

  template<typename KeyType, typename KeyNodePtrCompare>
  static node_ptr upper_bound_loop
      (node_ptr x, node_ptr y, const KeyType &key, KeyNodePtrCompare comp)
  {
    while(x){
      if(comp(key, x)){
        y = x;
        x = NodeTraits::get_left(x);
      }
      else{
        x = NodeTraits::get_right(x);
      }
    }
    return y;
  }

  template<typename Checker>
  static void check_subtree(const_node_ptr n, Checker checker, typename Checker::return_type& check_return)
  {
    const_node_ptr left = NodeTraits::get_left(n);
    const_node_ptr right = NodeTraits::get_right(n);
    typename Checker::return_type check_return_left;
    typename Checker::return_type check_return_right;
    if (left)
    {
      AUTODDS_ASSERT(NodeTraits::get_parent(left) == n);
      check_subtree(left, checker, check_return_left);
    }
    if (right)
    {
      AUTODDS_ASSERT(NodeTraits::get_parent(right) == n);
      check_subtree(right, checker, check_return_right);
    }
    checker(n, check_return_left, check_return_right, check_return);
  }

};

template<typename NodeTraits>
struct get_algo<BsTreeAlgorithms, NodeTraits>
{
  typedef bstree_algorithms<NodeTraits> type;
};

template<typename ValueTraits, typename NodePtrCompare, typename ExtraChecker>
struct get_node_checker<BsTreeAlgorithms, ValueTraits, NodePtrCompare, ExtraChecker>
{
  typedef detail::bstree_node_checker<ValueTraits, NodePtrCompare, ExtraChecker> type;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_BSTREE_ALGORITHMS_HPP_