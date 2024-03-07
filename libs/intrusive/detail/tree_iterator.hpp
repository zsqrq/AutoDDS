//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_ITERATOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/detail/std_fwd.hpp"
#include "libs/intrusive/detail/iiterator.hpp"
#include "libs/intrusive/detail/bstree_algorithms_base.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

// tree_iterator provides some basic functions for a
// node oriented bidirectional iterator:
template<typename ValueTraits, bool IsConst>
class tree_iterator
{
 private:
  typedef iiterator<ValueTraits, IsConst, std::bidirectional_iterator_tag>  types_t;
  typedef typename types_t::value_traits                value_traits;
  typedef typename types_t::node_traits                 node_traits;
  typedef typename types_t::node                        node;
  typedef typename types_t::node_ptr                    node_ptr;
  typedef typename types_t::const_value_traits_ptr      const_value_traits_ptr;
  typedef bstree_algorithms_base<node_traits>           node_algorithms;

  static const bool stateful_value_traits = types_t::stateful_value_traits;

  void unspecified_bool_type_func() const {}

  typedef void (tree_iterator::*unspecified_bool_type)() const;
  
  class nat;

  typedef typename
  autodds::intrusive::detail::if_c< IsConst
      , tree_iterator<value_traits, false>
      , nat>::type                           nonconst_iterator;

 public:
  typedef typename types_t::iterator_type::difference_type    difference_type;
  typedef typename types_t::iterator_type::value_type         value_type;
  typedef typename types_t::iterator_type::pointer            pointer;
  typedef typename types_t::iterator_type::reference          reference;
  typedef typename types_t::iterator_type::iterator_category  iterator_category;

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit tree_iterator(node_ptr nodeptr, const_value_traits_ptr traits_ptr)
      : members_(nodeptr, traits_ptr)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator(const tree_iterator &other)
      :  members_(other.pointed_node(), other.get_value_traits())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator(const nonconst_iterator &other)
      :  members_(other.pointed_node(), other.get_value_traits())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator &operator=(const tree_iterator &other)
  {  members_.nodeptr_ = other.members_.nodeptr_;  return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator &operator=(node_ptr nodeptr)
  {  members_.nodeptr_ = nodeptr;  return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE node_ptr pointed_node() const
  { return members_.nodeptr_; }

 public:
  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator& operator++()
  {
    members_.nodeptr_ = node_algorithms::next_node(members_.nodeptr_);
    return *this;
  }

  tree_iterator operator++(int)
  {
    tree_iterator result (*this);
    members_.nodeptr_ = node_algorithms::next_node(members_.nodeptr_);
    return result;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator& operator--()
  {
    members_.nodeptr_ = node_algorithms::prev_node(members_.nodeptr_);
    return *this;
  }

  tree_iterator operator--(int)
  {
    tree_iterator result (*this);
    members_.nodeptr_ = node_algorithms::prev_node(members_.nodeptr_);
    return result;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator&  go_left()
  {
    members_.nodeptr_ = node_traits::get_left(members_.nodeptr_);
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator&  go_right()
  {
    members_.nodeptr_ = node_traits::get_right(members_.nodeptr_);
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_iterator&  go_parent()
  {
    members_.nodeptr_ = node_traits::get_parent(members_.nodeptr_);
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE operator unspecified_bool_type() const
  {  return members_.nodeptr_ ? &tree_iterator::unspecified_bool_type_func : 0;   }

  AUTODDS_INTRUSIVE_FORCEINLINE bool operator! () const
  {  return !members_.nodeptr_;   }

  AUTODDS_INTRUSIVE_FORCEINLINE friend bool operator== (const tree_iterator& l, const tree_iterator& r)
  { return l.pointed_node() == r.pointed_node(); }

  AUTODDS_INTRUSIVE_FORCEINLINE friend bool operator!= (const tree_iterator& l, const tree_iterator& r)
  {  return !(l == r);   }

  AUTODDS_INTRUSIVE_FORCEINLINE reference operator*() const
  {  return *operator->();   }

  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator->() const
  { return this->operator_arrow(autodds::intrusive::detail::bool_<stateful_value_traits>()); }

  AUTODDS_INTRUSIVE_FORCEINLINE const_value_traits_ptr get_value_traits() const
  {  return members_.get_ptr();  }

  tree_iterator end_iterator_from_it() const
  {
    return tree_iterator(node_algorithms::get_header(this->pointed_node()), this->get_value_traits());
  }

  tree_iterator<value_traits, false> unconst() const
  {  return tree_iterator<value_traits, false>(this->pointed_node(), this->get_value_traits());   }

 private:
  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator_arrow(autodds::intrusive::detail::false_) const
  { return ValueTraits::to_value_ptr(members_.nodeptr_); }

  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator_arrow(autodds::intrusive::detail::true_) const
  { return this->get_value_traits()->to_value_ptr(members_.nodeptr_); }

  iiterator_members<node_ptr, const_value_traits_ptr, stateful_value_traits> members_;
};


} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_ITERATOR_HPP_