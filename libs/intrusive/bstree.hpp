//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/common/static_assert.hpp"
#include "libs/intrusive/bs_set_hook.hpp"
#include "libs/intrusive/detail/tree_node.hpp"
#include "libs/intrusive/detail/tree_iterator.hpp"
#include "libs/intrusive/detail/ebo_functor_holder.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/is_stateful_value_traits.hpp"
#include "libs/intrusive/detail/empty_node_checker.hpp"
#include "libs/intrusive/detail/default_header_holder.hpp"
#include "libs/intrusive/detail/reverse_iterator.hpp"
#include "libs/intrusive/detail/exception_disposer.hpp"
#include "libs/intrusive/detail/node_cloner_disposer.hpp"
#include "libs/intrusive/detail/key_nodeptr_comp.hpp"
#include "libs/intrusive/detail/simple_disposers.hpp"
#include "libs/intrusive/detail/size_holder.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/intrusive/detail/algorithm.hpp"
#include "libs/intrusive/detail/tree_value_compare.hpp"
#include "libs/intrusive/detail/get_value_traits.hpp"
#include "libs/intrusive/bstree_algorithms.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/parent_from_member.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/move/adl_move_swap.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

struct default_bstree_hook_applier
{
  template<class T>
  struct apply
  {
    typedef typename T::default_bstree_hook type;
  };
};

template<>
struct is_default_hook_tag<default_bstree_hook_applier>
{
  static const bool value = true;
};

struct bstree_defaults
{
  typedef default_bstree_hook_applier   proto_value_traits;
  static const bool constant_time_size = true;
  typedef std::size_t size_type;
  typedef void compare;
  typedef void key_of_value;
  static const bool floating_point = true;  //For sgtree
  typedef void priority;  //For treap
  typedef void header_holder_type;
};

template<typename ValueTraits, algo_types AlgoType, typename HeaderHolder>
struct bstbase3
{
  typedef ValueTraits                                               value_traits;
  typedef typename value_traits::node_traits                        node_traits;
  typedef typename node_traits::node                                node_type;
  typedef typename get_algo<AlgoType, node_traits>::type            node_algorithms;
  typedef typename node_traits::node_ptr                            node_ptr;
  typedef typename node_traits::const_node_ptr                      const_node_ptr;
  typedef tree_iterator<value_traits, false>                                                     iterator;
  typedef tree_iterator<value_traits, true>                                                      const_iterator;
  typedef autodds::libs::intrusive::reverse_iterator<iterator>                                   reverse_iterator;
  typedef autodds::libs::intrusive::reverse_iterator<const_iterator>                             const_reverse_iterator;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::pointer)                               pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::const_pointer)                         const_pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<pointer>::element_type)               value_type;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<pointer>::reference)                  reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::reference)            const_reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::difference_type)      difference_type;
  typedef typename detail::get_header_holder_type
      < value_traits,HeaderHolder >::type                                                       header_holder_type;

  static const bool safemode_or_autounlink = is_safe_autounlink<value_traits::link_mode>::value;
  static const bool stateful_value_traits = detail::is_stateful_value_traits<value_traits>::value;
  static const bool has_container_from_iterator =
      detail::is_same< header_holder_type, detail::default_header_holder< node_traits > >::value;

  struct holder_t : public ValueTraits
  {
    AUTODDS_INTRUSIVE_FORCEINLINE explicit holder_t(const ValueTraits &vtraits)
        : ValueTraits(vtraits)
    {}
    header_holder_type root;
  } holder;

  static bstbase3& get_tree_base_from_end_iterator(const const_iterator &end_iterator)
  {
    AUTODDS_STATIC_ASSERT(has_container_from_iterator);
    node_ptr p = end_iterator.pointed_node();
    header_holder_type* h = header_holder_type::get_holder(p);
    holder_t *holder = get_parent_from_member<holder_t, header_holder_type>(h, &holder_t::root);
    bstbase3 *base   = get_parent_from_member<bstbase3, holder_t> (holder, &bstbase3::holder);
    return *base;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE bstbase3(const ValueTraits &vtraits)
      : holder(vtraits)
  {
    node_algorithms::init_header(this->header_ptr());
  }

  AUTODDS_INTRUSIVE_FORCEINLINE node_ptr header_ptr()
  { return holder.root.get_node(); }

  AUTODDS_INTRUSIVE_FORCEINLINE const_node_ptr header_ptr() const
  { return holder.root.get_node(); }

  AUTODDS_INTRUSIVE_FORCEINLINE const value_traits &get_value_traits() const
  {  return this->holder;  }

  AUTODDS_INTRUSIVE_FORCEINLINE value_traits &get_value_traits()
  {  return this->holder;  }

  typedef typename autodds::libs::intrusive::value_traits_pointers
      <ValueTraits>::const_value_traits_ptr const_value_traits_ptr;

  AUTODDS_INTRUSIVE_FORCEINLINE const_value_traits_ptr priv_value_traits_ptr() const
  {  return pointer_traits<const_value_traits_ptr>::pointer_to(this->get_value_traits());  }

  AUTODDS_INTRUSIVE_FORCEINLINE iterator begin() AUTODDS_NOEXCEPT
  {  return iterator(node_algorithms::begin_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator begin() const AUTODDS_NOEXCEPT
  {  return cbegin();   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator cbegin() const AUTODDS_NOEXCEPT
  {  return const_iterator(node_algorithms::begin_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE iterator end() AUTODDS_NOEXCEPT
  {  return iterator(node_algorithms::end_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator end() const AUTODDS_NOEXCEPT
  {  return cend();  }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator cend() const AUTODDS_NOEXCEPT
  {  return const_iterator(node_algorithms::end_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE iterator root()
  {  return iterator(node_algorithms::root_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator root() const
  {  return croot();   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator croot() const
  {  return const_iterator(node_algorithms::root_node(this->header_ptr()), this->priv_value_traits_ptr());   }

  AUTODDS_INTRUSIVE_FORCEINLINE reverse_iterator rbegin()
  {  return reverse_iterator(end());  }

  AUTODDS_INTRUSIVE_FORCEINLINE const_reverse_iterator rbegin() const
  {  return const_reverse_iterator(end());  }

  AUTODDS_INTRUSIVE_FORCEINLINE const_reverse_iterator crbegin() const
  {  return const_reverse_iterator(end());  }

  AUTODDS_INTRUSIVE_FORCEINLINE reverse_iterator rend()
  {  return reverse_iterator(begin());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_reverse_iterator rend() const
  {  return const_reverse_iterator(begin());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_reverse_iterator crend() const
  {  return const_reverse_iterator(begin());   }

  void replace_node(iterator replace_this, reference with_this)
  {
    node_algorithms::replace_node( get_value_traits().to_node_ptr(*replace_this)
        , this->header_ptr()
        , get_value_traits().to_node_ptr(with_this));
    AUTODDS_IF_CONSTEXPR(safemode_or_autounlink)
      node_algorithms::init(replace_this.pointed_node());
  }

  AUTODDS_INTRUSIVE_FORCEINLINE void rebalance() AUTODDS_NOEXCEPT
  {  node_algorithms::rebalance(this->header_ptr()); }

  iterator rebalance_subtree(iterator r) AUTODDS_NOEXCEPT
  {  return iterator(node_algorithms::rebalance_subtree(r.pointed_node()), this->priv_value_traits_ptr()); }

  static iterator s_iterator_to(reference value) AUTODDS_NOEXCEPT
  {
    AUTODDS_STATIC_ASSERT((!stateful_value_traits));
    return iterator (value_traits::to_node_ptr(value), const_value_traits_ptr());
  }

  static const_iterator s_iterator_to(const_reference value) AUTODDS_NOEXCEPT
  {
    AUTODDS_STATIC_ASSERT((!stateful_value_traits));
    return const_iterator (value_traits::to_node_ptr(*pointer_traits<pointer>::const_cast_from(pointer_traits<const_pointer>::pointer_to(value))), const_value_traits_ptr());
  }

  iterator iterator_to(reference value) AUTODDS_NOEXCEPT
  {  return iterator (this->get_value_traits().to_node_ptr(value), this->priv_value_traits_ptr()); }

  const_iterator iterator_to(const_reference value) const AUTODDS_NOEXCEPT
  {  return const_iterator (this->get_value_traits().to_node_ptr(*pointer_traits<pointer>::const_cast_from(pointer_traits<const_pointer>::pointer_to(value))), this->priv_value_traits_ptr()); }

  AUTODDS_INTRUSIVE_FORCEINLINE static void init_node(reference value)
  { node_algorithms::init(value_traits::to_node_ptr(value)); }

};

template<typename Less, typename T>
struct get_compare
{
  typedef Less type;
};

template<typename T>
struct get_compare<void, T>
{
  typedef ::std::less<T> type;
};

template<typename KeyOfValue, typename T>
struct get_key_of_value
{
  typedef KeyOfValue type;
};

template<typename T>
struct get_key_of_value<void, T>
{
  typedef ::autodds::intrusive::detail::identity<T> type;
};

template<typename ValuePtr, typename VoidOrKeyOfValue, typename VoidOrKeyComp>
struct bst_key_types
{
  typedef typename
  autodds::movelib::pointer_element<ValuePtr>::type value_type;
  typedef typename get_key_of_value
      < VoidOrKeyOfValue, value_type>::type           key_of_value;
  typedef typename key_of_value::type                 key_type;
  typedef typename get_compare< VoidOrKeyComp
      , key_type
  >::type                         key_compare;
  typedef tree_value_compare
      <ValuePtr, key_compare, key_of_value>           value_compare;
};

template<typename ValueTraits, typename VoidOrKeyOfValue, typename VoidOrKeyComp, algo_types AlgoType, typename HeaderHolder>
struct bstbase2 : public detail::ebo_functor_holder<typename bst_key_types<
    typename ValueTraits::pointer, VoidOrKeyOfValue, VoidOrKeyComp>::value_compare>,
                  public bstbase3<ValueTraits, AlgoType, HeaderHolder>
{
  typedef bstbase3<ValueTraits, AlgoType, HeaderHolder>             treeheader_t;
  typedef bst_key_types< typename ValueTraits::pointer
      , VoidOrKeyOfValue, VoidOrKeyComp>                            key_types;
  typedef typename treeheader_t::value_traits                       value_traits;
  typedef typename treeheader_t::node_algorithms                    node_algorithms;
  typedef typename ValueTraits::value_type                          value_type;
  typedef typename key_types::key_type                              key_type;
  typedef typename key_types::key_of_value                          key_of_value;
  typedef typename key_types::key_compare                           key_compare;
  typedef typename key_types::value_compare                         value_compare;
  typedef typename treeheader_t::iterator                           iterator;
  typedef typename treeheader_t::const_iterator                     const_iterator;
  typedef typename treeheader_t::node_ptr                           node_ptr;
  typedef typename treeheader_t::const_node_ptr                     const_node_ptr;

  bstbase2(const key_compare &comp, const ValueTraits &vtraits)
      : detail::ebo_functor_holder<value_compare>(value_compare(comp)), treeheader_t(vtraits)
  {}

  const value_compare &get_comp() const
  {  return this->get();  }

  value_compare &get_comp()
  {  return this->get();  }

  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::pointer)                               pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::const_pointer)                         const_pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<pointer>::reference)                  reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::reference)            const_reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::difference_type)      difference_type;
  typedef typename node_algorithms::insert_commit_data insert_commit_data;

  AUTODDS_INTRUSIVE_FORCEINLINE value_compare value_comp() const
  {  return this->get_comp();   }

  AUTODDS_INTRUSIVE_FORCEINLINE key_compare key_comp() const
  {  return this->get_comp().key_comp();   }

  //lower_bound
  AUTODDS_INTRUSIVE_FORCEINLINE iterator lower_bound(const key_type &key)
  {  return this->lower_bound(key, this->key_comp());   }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator lower_bound(const key_type &key) const
  {  return this->lower_bound(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  iterator lower_bound(const KeyType &key, KeyTypeKeyCompare comp)
  {
    return iterator(node_algorithms::lower_bound
                        (this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  template<class KeyType, class KeyTypeKeyCompare>
  const_iterator lower_bound(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    return const_iterator(node_algorithms::lower_bound
                              (this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  //upper_bound
  AUTODDS_INTRUSIVE_FORCEINLINE iterator upper_bound(const key_type &key)
  {  return this->upper_bound(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  iterator upper_bound(const KeyType &key, KeyTypeKeyCompare comp)
  {
    return iterator(node_algorithms::upper_bound
                        (this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator upper_bound(const key_type &key) const
  {  return this->upper_bound(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  const_iterator upper_bound(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    return const_iterator(node_algorithms::upper_bound
                              (this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  template<class KeyTypeKeyCompare>
  struct key_node_comp_ret
  {  typedef detail::key_nodeptr_comp<KeyTypeKeyCompare, value_traits, key_of_value> type;  };

  template<class KeyTypeKeyCompare>
  AUTODDS_INTRUSIVE_FORCEINLINE typename key_node_comp_ret<KeyTypeKeyCompare>::type key_node_comp(KeyTypeKeyCompare comp) const
  {
    return detail::key_nodeptr_comp<KeyTypeKeyCompare, value_traits, key_of_value>(comp, &this->get_value_traits());
  }

  //find
  AUTODDS_INTRUSIVE_FORCEINLINE iterator find(const key_type &key)
  {  return this->find(key, this->key_comp()); }

  template<class KeyType, class KeyTypeKeyCompare>
  iterator find(const KeyType &key, KeyTypeKeyCompare comp)
  {
    return iterator
        (node_algorithms::find(this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  AUTODDS_INTRUSIVE_FORCEINLINE const_iterator find(const key_type &key) const
  {  return this->find(key, this->key_comp()); }

  template<class KeyType, class KeyTypeKeyCompare>
  const_iterator find(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    return const_iterator
        (node_algorithms::find(this->header_ptr(), key, this->key_node_comp(comp)), this->priv_value_traits_ptr());
  }

  //equal_range
  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<iterator,iterator> equal_range(const key_type &key)
  {  return this->equal_range(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator,iterator> equal_range(const KeyType &key, KeyTypeKeyCompare comp)
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::equal_range(this->header_ptr(), key, this->key_node_comp(comp));
    return std::pair<iterator, iterator>( iterator(ret.first, this->priv_value_traits_ptr())
        , iterator(ret.second, this->priv_value_traits_ptr()));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<const_iterator, const_iterator>
  equal_range(const key_type &key) const
  {  return this->equal_range(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<const_iterator, const_iterator>
  equal_range(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::equal_range(this->header_ptr(), key, this->key_node_comp(comp));
    return std::pair<const_iterator, const_iterator>( const_iterator(ret.first, this->priv_value_traits_ptr())
        , const_iterator(ret.second, this->priv_value_traits_ptr()));
  }

  //lower_bound_range
  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<iterator,iterator> lower_bound_range(const key_type &key)
  {  return this->lower_bound_range(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator,iterator> lower_bound_range(const KeyType &key, KeyTypeKeyCompare comp)
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::lower_bound_range(this->header_ptr(), key, this->key_node_comp(comp));
    return std::pair<iterator, iterator>( iterator(ret.first, this->priv_value_traits_ptr())
        , iterator(ret.second, this->priv_value_traits_ptr()));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<const_iterator, const_iterator>
  lower_bound_range(const key_type &key) const
  {  return this->lower_bound_range(key, this->key_comp());   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<const_iterator, const_iterator>
  lower_bound_range(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::lower_bound_range(this->header_ptr(), key, this->key_node_comp(comp));
    return std::pair<const_iterator, const_iterator>( const_iterator(ret.first, this->priv_value_traits_ptr())
        , const_iterator(ret.second, this->priv_value_traits_ptr()));
  }

  //bounded_range
  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<iterator,iterator> bounded_range
      (const key_type &lower_key, const key_type &upper_key, bool left_closed, bool right_closed)
  {  return this->bounded_range(lower_key, upper_key, this->key_comp(), left_closed, right_closed);   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator,iterator> bounded_range
      (const KeyType &lower_key, const KeyType &upper_key, KeyTypeKeyCompare comp, bool left_closed, bool right_closed)
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::bounded_range
            (this->header_ptr(), lower_key, upper_key, this->key_node_comp(comp), left_closed, right_closed);
    return std::pair<iterator, iterator>( iterator(ret.first, this->priv_value_traits_ptr())
        , iterator(ret.second, this->priv_value_traits_ptr()));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<const_iterator,const_iterator> bounded_range
      (const key_type &lower_key, const key_type &upper_key, bool left_closed, bool right_closed) const
  {  return this->bounded_range(lower_key, upper_key, this->key_comp(), left_closed, right_closed);   }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<const_iterator,const_iterator> bounded_range
      (const KeyType &lower_key, const KeyType &upper_key, KeyTypeKeyCompare comp, bool left_closed, bool right_closed) const
  {
    std::pair<node_ptr, node_ptr> ret =
        node_algorithms::bounded_range
            (this->header_ptr(), lower_key, upper_key, this->key_node_comp(comp), left_closed, right_closed);
    return std::pair<const_iterator, const_iterator>( const_iterator(ret.first, this->priv_value_traits_ptr())
        , const_iterator(ret.second, this->priv_value_traits_ptr()));
  }

  //insert_unique_check
  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<iterator, bool> insert_unique_check
      (const key_type &key, insert_commit_data &commit_data)
  {  return this->insert_unique_check(key, this->key_comp(), commit_data);   }

  AUTODDS_INTRUSIVE_FORCEINLINE std::pair<iterator, bool> insert_unique_check
      (const_iterator hint, const key_type &key, insert_commit_data &commit_data)
  {  return this->insert_unique_check(hint, key, this->key_comp(), commit_data);   }

  template<class KeyType, class KeyTypeKeyCompare>
  AUTODDS_INTRUSIVE_DOC1ST(std::pair<iterator AUTODDS_INTRUSIVE_I bool>
  , typename autodds::intrusive::detail::disable_if_convertible
      <KeyType AUTODDS_INTRUSIVE_I const_iterator AUTODDS_INTRUSIVE_I
                               std::pair<iterator AUTODDS_INTRUSIVE_I bool> >::type)
  insert_unique_check
      (const KeyType &key, KeyTypeKeyCompare comp, insert_commit_data &commit_data)
  {
    std::pair<node_ptr, bool> ret =
        (node_algorithms::insert_unique_check
            (this->header_ptr(), key, this->key_node_comp(comp), commit_data));
    return std::pair<iterator, bool>(iterator(ret.first, this->priv_value_traits_ptr()), ret.second);
  }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator, bool> insert_unique_check
      (const_iterator hint, const KeyType &key, KeyTypeKeyCompare comp, insert_commit_data &commit_data)
  {
    std::pair<node_ptr, bool> ret =
        (node_algorithms::insert_unique_check
            (this->header_ptr(), hint.pointed_node(), key, this->key_node_comp(comp), commit_data));
    return std::pair<iterator, bool>(iterator(ret.first, this->priv_value_traits_ptr()), ret.second);
  }
};

template<class ValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, bool ConstantTimeSize, class SizeType, algo_types AlgoType, typename HeaderHolder>
struct bstbase_hack:
    public detail::size_holder<ConstantTimeSize, SizeType>,
    public bstbase2 < ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, AlgoType, HeaderHolder>
{
  typedef bstbase2< ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, AlgoType, HeaderHolder> base_type;
  typedef typename base_type::key_compare         key_compare;
  typedef typename base_type::value_compare       value_compare;
  typedef SizeType                                size_type;
  typedef typename base_type::node_traits         node_traits;
  typedef typename get_algo
      <AlgoType, node_traits>::type                algo_type;

  AUTODDS_INTRUSIVE_FORCEINLINE bstbase_hack(const key_compare & comp, const ValueTraits &vtraits)
      : base_type(comp, vtraits)
  {
    this->sz_traits().set_size(size_type(0));
  }

  typedef detail::size_holder<ConstantTimeSize, SizeType>     size_traits;

  AUTODDS_INTRUSIVE_FORCEINLINE size_traits &sz_traits()
  {  return static_cast<size_traits &>(*this);  }

  AUTODDS_INTRUSIVE_FORCEINLINE const size_traits &sz_traits() const
  {  return static_cast<const size_traits &>(*this);  }
};

//Specialization for ConstantTimeSize == false
template<class ValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, class SizeType, algo_types AlgoType, typename HeaderHolder>
struct bstbase_hack<ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, false, SizeType, AlgoType, HeaderHolder>
    : public bstbase2 < ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, AlgoType, HeaderHolder>
{
  typedef bstbase2< ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, AlgoType, HeaderHolder> base_type;
  typedef typename base_type::value_compare       value_compare;
  typedef typename base_type::key_compare         key_compare;
  AUTODDS_INTRUSIVE_FORCEINLINE bstbase_hack(const key_compare & comp, const ValueTraits &vtraits)
      : base_type(comp, vtraits)
  {}

  typedef detail::size_holder<false, SizeType>     size_traits;

  AUTODDS_INTRUSIVE_FORCEINLINE size_traits sz_traits() const
  {  return size_traits();  }
};

template<class ValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, bool ConstantTimeSize, class SizeType, algo_types AlgoType, typename HeaderHolder>
struct bstbase
    : public bstbase_hack< ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, ConstantTimeSize, SizeType, AlgoType, HeaderHolder>
{
  typedef bstbase_hack< ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, ConstantTimeSize, SizeType, AlgoType, HeaderHolder> base_type;
  typedef ValueTraits                             value_traits;
  typedef typename base_type::value_compare       value_compare;
  typedef typename base_type::key_compare         key_compare;
  typedef typename base_type::const_reference     const_reference;
  typedef typename base_type::reference           reference;
  typedef typename base_type::iterator            iterator;
  typedef typename base_type::const_iterator      const_iterator;
  typedef typename base_type::node_traits         node_traits;
  typedef typename get_algo
      <AlgoType, node_traits>::type                node_algorithms;
  typedef SizeType                                size_type;

  AUTODDS_INTRUSIVE_FORCEINLINE bstbase(const key_compare & comp, const ValueTraits &vtraits)
      : base_type(comp, vtraits)
  {}

  //Detach all inserted nodes. This will add exception safety to bstree_impl
  //constructors inserting elements.
  ~bstbase()
  {
    if(is_safe_autounlink<value_traits::link_mode>::value){
      node_algorithms::clear_and_dispose
          ( this->header_ptr()
              , detail::node_disposer<detail::null_disposer, value_traits, AlgoType>
                (detail::null_disposer(), &this->get_value_traits()));
      node_algorithms::init(this->header_ptr());
    }
  }
};

//! The class template bstree is an unbalanced intrusive binary search tree
//! container. The no-throw guarantee holds only, if the key_compare object
//! doesn't throw.
template<class ValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, class SizeType, bool ConstantTimeSize, algo_types AlgoType, typename HeaderHolder>
class bstree_impl
    :  public bstbase<ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, ConstantTimeSize, SizeType, AlgoType, HeaderHolder>
{
 public:
  /// @cond
  typedef bstbase<ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, ConstantTimeSize, SizeType, AlgoType, HeaderHolder> data_type;
  typedef tree_iterator<ValueTraits, false> iterator_type;
  typedef tree_iterator<ValueTraits, true>  const_iterator_type;
  /// @endcond

  typedef AUTODDS_INTRUSIVE_IMPDEF(ValueTraits)                                                  value_traits;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::pointer)                               pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::const_pointer)                         const_pointer;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<pointer>::element_type)               value_type;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename data_type::key_type)                                 key_type;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename data_type::key_of_value)                             key_of_value;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<pointer>::reference)                  reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::reference)            const_reference;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename pointer_traits<const_pointer>::difference_type)      difference_type;
  typedef AUTODDS_INTRUSIVE_IMPDEF(SizeType)                                                     size_type;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename data_type::value_compare)                            value_compare;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename data_type::key_compare)                              key_compare;
  typedef AUTODDS_INTRUSIVE_IMPDEF(iterator_type)                                                iterator;
  typedef AUTODDS_INTRUSIVE_IMPDEF(const_iterator_type)                                          const_iterator;
  typedef AUTODDS_INTRUSIVE_IMPDEF(autodds::libs::intrusive::reverse_iterator<iterator>)         reverse_iterator;
  typedef AUTODDS_INTRUSIVE_IMPDEF(autodds::libs::intrusive::reverse_iterator<const_iterator>)   const_reverse_iterator;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename value_traits::node_traits)                           node_traits;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename node_traits::node)                                   node;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename node_traits::node_ptr)                               node_ptr;
  typedef AUTODDS_INTRUSIVE_IMPDEF(typename node_traits::const_node_ptr)                         const_node_ptr;
  /// @cond
  typedef typename get_algo<AlgoType, node_traits>::type                                       algo_type;
  /// @endcond
  typedef AUTODDS_INTRUSIVE_IMPDEF(algo_type)                                                    node_algorithms;

  static const bool constant_time_size = ConstantTimeSize;
  static const bool stateful_value_traits = detail::is_stateful_value_traits<value_traits>::value;

 private:

  //noncopyable
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(bstree_impl)

  static const bool safemode_or_autounlink = is_safe_autounlink<value_traits::link_mode>::value;

  //Constant-time size is incompatible with auto-unlink hooks!
  AUTODDS_STATIC_ASSERT(!(constant_time_size && ((int)value_traits::link_mode == (int)auto_unlink)));

 public:

  typedef typename node_algorithms::insert_commit_data insert_commit_data;

  //! <b>Effects</b>: Constructs an empty container.
  bstree_impl()
      :  data_type(key_compare(), value_traits())
  {}

  //! <b>Effects</b>: Constructs an empty container with given comparison and traits.
  explicit bstree_impl( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  data_type(cmp, v_traits)
  {}

  //! <b>Effects</b>: Constructs an empty container and inserts elements from
  //!   [b, e).
  template<typename Iterator>
  bstree_impl( bool unique, Iterator b, Iterator e
      , const key_compare &cmp     = key_compare()
      , const value_traits &v_traits = value_traits())
      : data_type(cmp, v_traits)
  {
    //bstbase releases elements in case of exceptions
    if(unique)
      this->insert_unique(b, e);
    else
      this->insert_equal(b, e);
  }

  //! <b>Effects</b>: Constructs a container moving resources from another container.
  //!   Internal comparison object and value traits are move constructed and
  //!   nodes belonging to x (except the node representing the "end") are linked to *this.
  bstree_impl(AUTODDS_RV_REF(bstree_impl) x)
      : data_type(::autodds::libs::move(x.get_comp()), ::autodds::libs::move(x.get_value_traits()))
  {
    this->swap(x);
  }

  //! <b>Effects</b>: Equivalent to swap
  //!
  AUTODDS_INTRUSIVE_FORCEINLINE bstree_impl& operator =(AUTODDS_RV_REF(bstree_impl) x)
  {  this->swap(x); return *this;  }

  //! <b>Effects</b>: Returns a const reference to the container associated to the end iterator
  static bstree_impl &container_from_end_iterator(iterator end_iterator) AUTODDS_NOEXCEPT
  {
    return static_cast<bstree_impl&>
    (data_type::get_tree_base_from_end_iterator(end_iterator));
  }

  //! <b>Effects</b>: Returns a const reference to the container associated to the iterator
  static const bstree_impl &container_from_end_iterator(const_iterator end_iterator) AUTODDS_NOEXCEPT
  {
    return static_cast<bstree_impl&>
    (data_type::get_tree_base_from_end_iterator(end_iterator));
  }

  //! <b>Effects</b>: Returns a const reference to the container associated to the iterator
  static bstree_impl& container_from_iterator(iterator it) AUTODDS_NOEXCEPT
  {
    return container_from_end_iterator(it.end_iterator_from_it());
  }

  //! <b>Effects</b>: Returns a const reference to the container associated to the end iterator
  static const bstree_impl& container_from_iterator(const_iterator it) AUTODDS_NOEXCEPT
  {
    return container_from_end_iterator(it.end_iterator_from_it());
  }

  //! <b>Effects</b>: Returns true if the container is empty.
  bool empty() const AUTODDS_NOEXCEPT
  {
    if (ConstantTimeSize)
    {
      return !this->data_type::sz_traits().get_size();
    } else
    {
      return algo_type::unique(this->header_ptr());
    }
  }

  //! <b>Effects</b>: Returns the number of elements stored in the container.
  size_type size() const AUTODDS_NOEXCEPT
  {
    AUTODDS_IF_CONSTEXPR(constant_time_size)
      return this->sz_traits().get_size();
    else{
      return (size_type)node_algorithms::size(this->header_ptr());
    }
  }

  //! <b>Effects</b>: Swaps the contents of two containers.
  void swap(bstree_impl& other)
  {
    ::autodds::libs::adl_move_swap(this->get_comp(), other.get_comp());
    node_algorithms::swap_tree(this->header_ptr(), node_ptr(other.header_ptr()));
    this->sz_traits().swap(other.sz_traits());
  }

  //! <b>Effects</b>: Erases all the elements from *this
  //!   calling Disposer::operator()(pointer), clones all the
  //!   elements from src calling Cloner::operator()(const_reference )
  //!   and inserts them on *this. Copies the predicate from the source container.
  template<typename Cloner, typename Disposer>
  void clone_from(const bstree_impl& src, Cloner cloner, Disposer disposer)
  {
    this->clear_and_dispose(disposer);
    if(!src.empty()){
      detail::exception_disposer<bstree_impl, Disposer> rollback(*this, disposer);
      node_algorithms::clone(
          src.header_ptr(),
          this->header_ptr(),
          detail::node_cloner <Cloner, value_traits, AlgoType>(cloner, &this->get_value_traits()),
          detail::node_disposer<Disposer, value_traits, AlgoType>(disposer, &this->get_value_traits()));
      this->sz_traits().set_size(src.sz_traits().get_size());
      this->get_comp() = src.get_comp();
      rollback.release();
    }
  }

  //! <b>Effects</b>: Erases all the elements from *this
  //!   calling Disposer::operator()(pointer), clones all the
  //!   elements from src calling Cloner::operator()(reference)
  //!   and inserts them on *this. Copies the predicate from the source container.
  template <class Cloner, class Disposer>
  void clone_from(AUTODDS_RV_REF(bstree_impl) src, Cloner cloner, Disposer disposer)
  {
    this->clear_and_dispose(disposer);
    if(!src.empty()){
      detail::exception_disposer<bstree_impl, Disposer> rollback(*this, disposer);
      node_algorithms::clone
          (src.header_ptr(),
           this->header_ptr(),
           detail::node_cloner <Cloner,    value_traits, AlgoType, false>(cloner,   &this->get_value_traits()),
           detail::node_disposer<Disposer, value_traits, AlgoType>(disposer, &this->get_value_traits()));
      this->sz_traits().set_size(src.sz_traits().get_size());
      this->get_comp() = src.get_comp();
      rollback.release();
    }
  }

  //! <b>Effects</b>: Inserts value into the container before the upper bound.
  iterator insert_equal(reference value)
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));
    iterator ret(node_algorithms::insert_equal_upper_bound
                     (this->header_ptr(), to_insert, this->key_node_comp(this->key_comp())), this->priv_value_traits_ptr());
    this->sz_traits().increment();
    return ret;
  }

  //! <b>Effects</b>: Inserts x into the container, using "hint" as a hint to
  //!   where it will be inserted. If "hint" is the upper_bound
  //!   the insertion takes constant time (two comparisons in the worst case)
  iterator insert_equal(const_iterator hint, reference value)
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));
    iterator ret(node_algorithms::insert_equal
                     (this->header_ptr(), hint.pointed_node(), to_insert, this->key_node_comp(this->key_comp())), this->priv_value_traits_ptr());
    this->sz_traits().increment();
    return ret;
  }

  //! <b>Effects</b>: Inserts a each element of a range into the container
  //!   before the upper bound of the key of each element.
  template<typename Iterator>
  void insert_equal(Iterator b, Iterator e)
  {
    iterator iend(this->end());
    for (; b != e; ++b)
      this->insert_equal(iend, *b);
  }

  //! <b>Effects</b>: Inserts value into the container if the value
  //!   is not already present.
  std::pair<iterator, bool> insert_unique(reference value)
  {
    insert_commit_data commit_data;
    std::pair<node_ptr, bool> ret =(node_algorithms::insert_unique_check
        (this->header_ptr(), key_of_value()(value), this->key_node_comp(this->key_comp()), commit_data));
    return std::pair<iterator, bool>
        ( ret.second ? this->insert_unique_commit(value, commit_data)
                     : iterator(ret.first, this->priv_value_traits_ptr())
            , ret.second);
  }

  //! <b>Effects</b>: Tries to insert x into the container, using "hint" as a hint
  //!   to where it will be inserted.
  iterator insert_unique(const_iterator hint, reference value)
  {
    insert_commit_data commit_data;
    std::pair<node_ptr, bool> ret =
        (node_algorithms::insert_unique_check
            (this->header_ptr(), hint.pointed_node(), key_of_value()(value), this->key_node_comp(this->key_comp()), commit_data));
    return ret.second ? this->insert_unique_commit(value, commit_data)
                      : iterator(ret.first, this->priv_value_traits_ptr());
  }

  //! <b>Effects</b>: Tries to insert each element of a range into the container.
  template<typename Iterator>
  void insert_unique(Iterator b, Iterator e)
  {
    if(this->empty()){
      iterator iend(this->end());
      for (; b != e; ++b)
        this->insert_unique(iend, *b);
    }
    else{
      for (; b != e; ++b)
        this->insert_unique(*b);
    }
  }

  //! <b>Effects</b>: Inserts the value in the container using the information obtained
  //!   from the "commit_data" that a previous "insert_check" filled.
  iterator insert_unique_commit(reference value, const insert_commit_data &commit_data) AUTODDS_NOEXCEPT
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));

#if !(defined(AUTODDS_DISABLE_ASSERTS) || ( defined(AUTODDS_ENABLE_ASSERT_DEBUG_HANDLER) && defined(NDEBUG) ))
    //Test insertion position is correct
    iterator p(commit_data.node, this->priv_value_traits_ptr());
    if(!commit_data.link_left){
      ++p;
    }
    //Check if the insertion point is correct to detect wrong
    //uses insert_unique_check
    AUTODDS_ASSERT(( p == this->end()   || !this->get_comp()(*p, value)   ));
    AUTODDS_ASSERT(( p == this->begin() || !this->get_comp()(value, *--p) ));
#endif

    node_algorithms::insert_unique_commit
        (this->header_ptr(), to_insert, commit_data);
    this->sz_traits().increment();
    return iterator(to_insert, this->priv_value_traits_ptr());
  }

  //! <b>Effects</b>: Inserts x into the container before "pos".
  iterator insert_before(const_iterator pos, reference value) AUTODDS_NOEXCEPT
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));
    this->sz_traits().increment();
    return iterator(node_algorithms::insert_before
                        (this->header_ptr(), pos.pointed_node(), to_insert), this->priv_value_traits_ptr());
  }

  //! <b>Effects</b>: Inserts x into the container in the last position.
  void push_back(reference value) AUTODDS_NOEXCEPT
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));
    this->sz_traits().increment();
    node_algorithms::push_back(this->header_ptr(), to_insert);
  }

  //! <b>Effects</b>: Inserts x into the container in the first position.
  void push_front(reference value) AUTODDS_NOEXCEPT
  {
    node_ptr to_insert(this->get_value_traits().to_node_ptr(value));
    AUTODDS_ASSERT(!safemode_or_autounlink || node_algorithms::unique(to_insert));
    this->sz_traits().increment();
    node_algorithms::push_front(this->header_ptr(), to_insert);
  }

  //! <b>Effects</b>: Erases the element pointed to by i.
  iterator erase(const_iterator i) AUTODDS_NOEXCEPT
  {
    const_iterator ret(i);
    ++ret;
    node_ptr to_erase(i.pointed_node());
    AUTODDS_ASSERT(!safemode_or_autounlink || !node_algorithms::unique(to_erase));
    node_algorithms::erase(this->header_ptr(), to_erase);
    this->sz_traits().decrement();
    AUTODDS_IF_CONSTEXPR(safemode_or_autounlink)
      node_algorithms::init(to_erase);
    return ret.unconst();
  }

  //! <b>Effects</b>: Erases the range pointed to by b end e.
  iterator erase(const_iterator b, const_iterator e) AUTODDS_NOEXCEPT
  {  size_type n;   return this->private_erase(b, e, n);   }

  //! <b>Effects</b>: Erases all the elements with the given value.
  size_type erase(const key_type &key) AUTODDS_NOEXCEPT
  {  return this->erase(key, this->key_comp());   }

  //! <b>Effects</b>: Erases all the elements with the given key.
  //!   according to the comparison functor "comp".
  template<typename KeyType, typename KeyTypeKeyCompare>
  AUTODDS_INTRUSIVE_DOC1ST(size_type
  , typename autodds::intrusive::detail::disable_if_convertible<KeyTypeKeyCompare AUTODDS_INTRUSIVE_I const_iterator AUTODDS_INTRUSIVE_I size_type>::type)
  erase(const KeyType& key, KeyTypeKeyCompare comp)
  {
    std::pair<iterator,iterator> p = this->equal_range(key, comp);
    size_type n;
    this->private_erase(p.first, p.second, n);
    return n;
  }

  //! <b>Effects</b>: Erases the element pointed to by i.
  template<typename Disposer>
  iterator erase_and_dispose(const_iterator i, Disposer disposer) AUTODDS_NOEXCEPT
  {
    node_ptr to_erase(i.pointed_node());
    iterator ret(this->erase(i));
    disposer(this->get_value_traits().to_value_ptr(to_erase));
    return ret;
  }

  //! <b>Effects</b>: Erases all the elements with the given value.
  //!   Disposer::operator()(pointer) is called for the removed elements.
  template<typename Disposer>
  size_type erase_and_dispose(const key_type &key, Disposer disposer)
  {
    std::pair<iterator,iterator> p = this->equal_range(key);
    size_type n;
    this->private_erase(p.first, p.second, n, disposer);
    return n;
  }

  template<typename Disposer>
  iterator erase_and_dispose(const_iterator b, const_iterator e, Disposer disposer) AUTODDS_NOEXCEPT
  {  size_type n;   return this->private_erase(b, e, n, disposer);   }

  //! <b>Requires</b>: key is a value such that `*this` is partitioned with respect to
  //!   comp(nk, key) and !comp(key, nk), with comp(nk, key) implying !comp(key, nk)
  //!   and nk the key_type of a value_type inserted into `*this`.
  template<class KeyType, class KeyTypeKeyCompare, class Disposer>
  AUTODDS_INTRUSIVE_DOC1ST(size_type
  , typename autodds::intrusive::detail::disable_if_convertible<KeyTypeKeyCompare AUTODDS_INTRUSIVE_I const_iterator AUTODDS_INTRUSIVE_I size_type>::type)
  erase_and_dispose(const KeyType& key, KeyTypeKeyCompare comp, Disposer disposer)
  {
    std::pair<iterator,iterator> p = this->equal_range(key, comp);
    size_type n;
    this->private_erase(p.first, p.second, n, disposer);
    return n;
  }

  void clear() AUTODDS_NOEXCEPT
  {
    AUTODDS_IF_CONSTEXPR(safemode_or_autounlink){
      this->clear_and_dispose(detail::null_disposer());
    }
    else{
      node_algorithms::init_header(this->header_ptr());
      this->sz_traits().set_size(0);
    }
  }

  //! <b>Effects</b>: Erases all of the elements calling disposer(p) for
  //!   each node to be erased.
  template<typename Disposer>
  void clear_and_dispose(Disposer disposer) AUTODDS_NOEXCEPT
  {
    node_algorithms::clear_and_dispose(this->header_ptr()
        , detail::node_disposer<Disposer, value_traits, AlgoType>(disposer, &this->get_value_traits()));
    node_algorithms::init_header(this->header_ptr());
    this->sz_traits().set_size(0);
  }

  //! <b>Effects</b>: Returns the number of contained elements with the given value
  size_type count(const key_type &key) const
  {  return size_type(this->count(key, this->key_comp()));   }

  template<typename KeyType, typename KeyTypeKeyCompare>
  size_type count(const KeyType &key, KeyTypeKeyCompare comp) const
  {
    std::pair<const_iterator, const_iterator> ret = this->equal_range(key, comp);
    size_type n = 0;
    for(; ret.first != ret.second; ++ret.first){ ++n; }
    return n;
  }

  size_type count(const key_type &key)
  {  return size_type(this->count(key, this->key_comp()));   }

  template<typename KeyType, typename KeyTypeKeyCompare>
  size_type count(const KeyType &key, KeyTypeKeyCompare comp)
  {
    std::pair<const_iterator, const_iterator> ret = this->equal_range(key, comp);
    size_type n = 0;
    for(; ret.first != ret.second; ++ret.first){ ++n; }
    return n;
  }

  //! <b>Effects</b>: Unlinks the leftmost node from the container.
  pointer unlink_leftmost_without_rebalance() AUTODDS_NOEXCEPT
  {
    node_ptr to_be_disposed(node_algorithms::unlink_leftmost_without_rebalance
                                (this->header_ptr()));
    if(!to_be_disposed)
      return 0;
    this->sz_traits().decrement();
    AUTODDS_IF_CONSTEXPR(safemode_or_autounlink)//If this is commented does not work with normal_link
      node_algorithms::init(to_be_disposed);
    return this->get_value_traits().to_value_ptr(to_be_disposed);
  }

  //! <b>Effects</b>: removes "value" from the container.
  static void remove_node(reference value) AUTODDS_NOEXCEPT
  {
    AUTODDS_STATIC_ASSERT((!constant_time_size));
    node_ptr to_remove(value_traits::to_node_ptr(value));
    node_algorithms::unlink(to_remove);
    AUTODDS_IF_CONSTEXPR(safemode_or_autounlink)
      node_algorithms::init(to_remove);
  }

  //! <b>Effects</b>: Attempts to extract each element in source and insert it into a using
  //!   the comparison object of *this. If there is an element in a with key equivalent to the
  //!   key of an element from source, then that element is not extracted from source.
  template<typename Compare2>
  void merge_unique(bstree_impl
                        <ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, AlgoType, HeaderHolder> &source)
  {
    node_ptr it   (node_algorithms::begin_node(source.header_ptr()))
    , itend(node_algorithms::end_node  (source.header_ptr()));

    while(it != itend){
      node_ptr const p(it);
      AUTODDS_ASSERT(!safemode_or_autounlink || !node_algorithms::unique(p));
      it = node_algorithms::next_node(it);
      if( node_algorithms::transfer_unique(this->header_ptr(), this->key_node_comp(this->key_comp()), source.header_ptr(), p) ){
        source.sz_traits().decrement();
        this->sz_traits().increment();
      }
    }
  }

  //! <b>Effects</b>: Extracts each element in source and insert it into a using
  //!   the comparison object of *this.
  template<class Compare2>
  void merge_equal(bstree_impl
                       <ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, AlgoType, HeaderHolder> &source)
  {
    node_ptr it   (node_algorithms::begin_node(source.header_ptr()))
    , itend(node_algorithms::end_node  (source.header_ptr()));

    while(it != itend){
      node_ptr const p(it);
          AUTODDS_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(!safemode_or_autounlink || !node_algorithms::unique(p));
      it = node_algorithms::next_node(it);
      node_algorithms::transfer_equal(this->header_ptr(), this->key_node_comp(this->key_comp()), source.header_ptr(), p);
      source.sz_traits().decrement();
      this->sz_traits().increment();
    }
  }

  //! <b>Effects</b>: Asserts the integrity of the container with additional checks provided by the user.
  template <typename ExtraChecker>
  void check(ExtraChecker extra_checker) const
  {
    typedef detail::key_nodeptr_comp<key_compare, value_traits, key_of_value> nodeptr_comp_t;
    nodeptr_comp_t nodeptr_comp(this->key_comp(), &this->get_value_traits());
    typedef typename get_node_checker<AlgoType, ValueTraits, nodeptr_comp_t, ExtraChecker>::type node_checker_t;
    typename node_checker_t::return_type checker_return;
    node_algorithms::check(this->header_ptr(), node_checker_t(nodeptr_comp, extra_checker), checker_return);
    AUTODDS_ASSERT(!constant_time_size || this->sz_traits().get_size() == checker_return.node_count);
  }

  //! <b>Effects</b>: Asserts the integrity of the container.
  void check() const
  {
    check(detail::empty_node_checker<ValueTraits>());
  }

  friend bool operator==(const bstree_impl &x, const bstree_impl &y)
  {
    if(constant_time_size && x.size() != y.size()){
      return false;
    }
    return autodds::libs::intrusive::algo_equal(x.cbegin(), x.cend(), y.cbegin(), y.cend());
  }

  friend bool operator!=(const bstree_impl &x, const bstree_impl &y)
  {  return !(x == y); }

  friend bool operator<(const bstree_impl &x, const bstree_impl &y)
  {  return ::autodds::libs::intrusive::algo_lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());  }

  friend bool operator>(const bstree_impl &x, const bstree_impl &y)
  {  return y < x;  }

  friend bool operator<=(const bstree_impl &x, const bstree_impl &y)
  {  return !(x > y);  }

  friend bool operator>=(const bstree_impl &x, const bstree_impl &y)
  {  return !(x < y);  }

  friend void swap(bstree_impl &x, bstree_impl &y)
  {  x.swap(y);  }

 private:
  template<class Disposer>
  iterator private_erase(const_iterator b, const_iterator e, size_type &n, Disposer disposer)
  {
    for(n = 0; b != e; ++n)
      this->erase_and_dispose(b++, disposer);
    return b.unconst();
  }

  iterator private_erase(const_iterator b, const_iterator e, size_type &n)
  {
    for(n = 0; b != e; ++n)
      this->erase(b++);
    return b.unconst();
  }

};

template<typename T, typename ...Options>
struct make_bstree
{
  typedef typename pack_options< bstree_defaults,Options...>::type  packed_options;

  typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type        value_traits;

  typedef bstree_impl
      < value_traits
          , typename packed_options::key_of_value
          , typename packed_options::compare
          , typename packed_options::size_type
          , packed_options::constant_time_size
          , BsTreeAlgorithms
          , typename packed_options::header_holder_type
      > implementation_defined;
  /// @endcond
  typedef implementation_defined type;
};

template<typename T, typename ...Options>
class bstree : public make_bstree<T, Options...>::type
{
  typedef typename make_bstree<T, Options...>::type   Base;

 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(bstree)
 
 public:
  typedef typename Base::key_compare        key_compare;
  typedef typename Base::value_traits       value_traits;
  typedef typename Base::iterator           iterator;
  typedef typename Base::const_iterator     const_iterator;

  //Assert if passed value traits are compatible with the type
  AUTODDS_STATIC_ASSERT((detail::is_same<typename value_traits::value_type, T>::value));

  AUTODDS_INTRUSIVE_FORCEINLINE bstree()
      :  Base()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit bstree( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
  {}

  template<class Iterator>
  AUTODDS_INTRUSIVE_FORCEINLINE bstree( bool unique, Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      :  Base(unique, b, e, cmp, v_traits)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE bstree(AUTODDS_RV_REF(bstree) x)
  :  Base(AUTODDS_MOVE_BASE(Base, x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE bstree& operator=(AUTODDS_RV_REF(bstree) x)
  {  return static_cast<bstree &>(this->Base::operator=(AUTODDS_MOVE_BASE(Base, x)));  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(const bstree &src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(src, cloner, disposer);  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(AUTODDS_RV_REF(bstree) src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(AUTODDS_MOVE_BASE(Base, src), cloner, disposer);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static bstree &container_from_end_iterator(iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<bstree &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const bstree &container_from_end_iterator(const_iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<const bstree &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static bstree &container_from_iterator(iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<bstree &>(Base::container_from_iterator(it));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const bstree &container_from_iterator(const_iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<const bstree &>(Base::container_from_iterator(it));   }

};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_BSTREE_HPP_