//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_IITERATOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_IITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/detail/iterator.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/is_stateful_value_traits.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename ValueTraits>
struct value_traits_pointers
{
  typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT
  (autodds::intrusive::detail::
  , ValueTraits, value_traits_ptr
  , typename autodds::libs::intrusive::pointer_traits<typename ValueTraits::node_traits::node_ptr>::template
       rebind_pointer<ValueTraits>::type)   value_traits_ptr;

  typedef typename autodds::libs::intrusive::pointer_traits<value_traits_ptr>::template
  rebind_pointer<ValueTraits const>::type const_value_traits_ptr;
};

template<typename ValueTraits, bool IsConst, class Category>
struct iiterator
{
  typedef ValueTraits                                         value_traits;
  typedef typename value_traits::node_traits                  node_traits;
  typedef typename node_traits::node                          node;
  typedef typename node_traits::node_ptr                      node_ptr;
  typedef ::autodds::libs::intrusive::pointer_traits<node_ptr>        nodepointer_traits_t;
  typedef typename nodepointer_traits_t::template
  rebind_pointer<void>::type                               void_pointer;
  typedef typename ValueTraits::value_type                    value_type;
  typedef typename ValueTraits::pointer                       nonconst_pointer;
  typedef typename ValueTraits::const_pointer                 yesconst_pointer;
  typedef typename ::autodds::libs::intrusive::pointer_traits
      <nonconst_pointer>::reference                            nonconst_reference;
  typedef typename ::autodds::libs::intrusive::pointer_traits
      <yesconst_pointer>::reference                            yesconst_reference;
  typedef typename nodepointer_traits_t::difference_type      difference_type;
  typedef typename autodds::intrusive::detail::if_c
      <IsConst, yesconst_pointer, nonconst_pointer>::type      pointer;
  typedef typename autodds::intrusive::detail::if_c
      <IsConst, yesconst_reference, nonconst_reference>::type  reference;
  typedef iterator< Category
      , value_type
      , difference_type
      , pointer
      , reference
  > iterator_type;
  typedef typename value_traits_pointers
      <ValueTraits>::value_traits_ptr                          value_traits_ptr;
  typedef typename value_traits_pointers
      <ValueTraits>::const_value_traits_ptr                    const_value_traits_ptr;
  static const bool stateful_value_traits =
      detail::is_stateful_value_traits<value_traits>::value;
};

template<typename NodePtr, typename StoredPointer, bool StatefulValueTraits = true>
struct iiterator_members
{

  AUTODDS_INTRUSIVE_FORCEINLINE iiterator_members()
      : nodeptr_()//Value initialization to achieve "null iterators" (N3644)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE iiterator_members(const NodePtr &n_ptr, const StoredPointer &data)
      :  nodeptr_(n_ptr), ptr_(data)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE StoredPointer get_ptr() const
  {  return ptr_;  }

  NodePtr nodeptr_;
  StoredPointer ptr_;
};

template<class NodePtr, class StoredPointer>
struct iiterator_members<NodePtr, StoredPointer, false>
{
  AUTODDS_INTRUSIVE_FORCEINLINE iiterator_members()
      : nodeptr_()//Value initialization to achieve "null iterators" (N3644)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE iiterator_members(const NodePtr &n_ptr, const StoredPointer &)
      : nodeptr_(n_ptr)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE StoredPointer get_ptr() const
  {  return StoredPointer();  }

  NodePtr nodeptr_;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_IITERATOR_HPP_