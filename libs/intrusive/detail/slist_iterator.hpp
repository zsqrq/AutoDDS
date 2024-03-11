//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_ITERATOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_ITERATOR_HPP_

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
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/common/static_assert.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename ValueTraits, bool IsConst>
class slist_iterator
{
 private:
  typedef iiterator<ValueTraits, IsConst, std::forward_iterator_tag>  types_t;
  
  static const bool stateful_value_traits =   types_t::stateful_value_traits;

  typedef ValueTraits                                      value_traits;
  typedef typename types_t::node_traits                    node_traits;

  typedef typename types_t::node                           node;
  typedef typename types_t::node_ptr                       node_ptr;
  typedef typename types_t::const_value_traits_ptr         const_value_traits_ptr; 
  
  class nat;
  typedef typename detail::if_c<IsConst, 
  slist_iterator<value_traits, false>, nat>::type          nonconst_iterator;
  
 public:
  typedef typename types_t::iterator_type::difference_type    difference_type;
  typedef typename types_t::iterator_type::value_type         value_type;
  typedef typename types_t::iterator_type::pointer            pointer;
  typedef typename types_t::iterator_type::reference          reference;
  typedef typename types_t::iterator_type::iterator_category  iterator_category;

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator(node_ptr nodeptr, const_value_traits_ptr traits_ptr)
      : members_(nodeptr, traits_ptr)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit slist_iterator(node_ptr nodeptr)
      : members_(nodeptr, const_value_traits_ptr())
  {  AUTODDS_STATIC_ASSERT((stateful_value_traits == false));  }

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator(const slist_iterator &other)
      :  members_(other.pointed_node(), other.get_value_traits())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator(const nonconst_iterator &other)
      :  members_(other.pointed_node(), other.get_value_traits())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator &operator=(const slist_iterator &other)
  {  members_.nodeptr_ = other.members_.nodeptr_;  return *this;  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE node_ptr pointed_node() const
  {
    return members_.nodeptr_;
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator& operator =(node_ptr other)
  {
    members_.nodeptr_ = other;
    return static_cast<slist_iterator&>(*this);
  }
  
  AUTODDS_INTRUSIVE_FORCEINLINE const_value_traits_ptr get_value_traits() const
  {  return members_.get_ptr(); }

  AUTODDS_INTRUSIVE_FORCEINLINE bool operator !() const
  {  return !members_.nodeptr_; }
  
 public:
  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator& operator ++()
  {
    members_.nodeptr_ = node_traits::get_next(members_.nodeptr_);
    return static_cast<slist_iterator&> (*this);
  }

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator operator++(int)
  {
    slist_iterator result (*this);
    members_.nodeptr_ = node_traits::get_next(members_.nodeptr_);
    return result;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE friend bool operator == (const slist_iterator& l, const slist_iterator& r)
  {  return l.pointed_node() == r.pointed_node();   }

  AUTODDS_INTRUSIVE_FORCEINLINE friend bool operator != (const slist_iterator& l, const slist_iterator& r)
  {  return l.pointed_node() != r.pointed_node();   }

  AUTODDS_INTRUSIVE_FORCEINLINE reference operator *() const
  {  return *operator->();   }

  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator ->() const
  { return this->operator_arrow(autodds::libs::intrusive::detail::bool_<stateful_value_traits>()); }

  AUTODDS_INTRUSIVE_FORCEINLINE slist_iterator<ValueTraits, false> unconst() const
  {  return slist_iterator<ValueTraits, false>(this->pointed_node(), this->get_value_traits());   }
  
 private:
  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator_arrow(autodds::libs::intrusive::detail::false_) const
  { return ValueTraits::to_value_ptr(members_.nodeptr_); }

  AUTODDS_INTRUSIVE_FORCEINLINE pointer operator_arrow(autodds::libs::intrusive::detail::true_) const
  { return this->get_value_traits()->to_value_ptr(members_.nodeptr_); }

  iiterator_members<node_ptr, const_value_traits_ptr, stateful_value_traits> members_;
};

}
}
}

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_SLIST_ITERATOR_HPP_