//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_SET_HPP_
#define AUTODDS_LIBS_INTRUSIVE_SET_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/rbtree.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/common/static_assert.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<class ValueTraits, class VoidOrKeyOfValue, class Compare, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
class multiset_impl;

template<class ValueTraits, class VoidOrKeyOfValue, class Compare, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
class set_impl:
    public bstree_impl<ValueTraits, VoidOrKeyOfValue, Compare, SizeType, ConstantTimeSize, RbTreeAlgorithms, HeaderHolder>
{
  typedef bstree_impl<ValueTraits, VoidOrKeyOfValue, Compare, SizeType, ConstantTimeSize, RbTreeAlgorithms, HeaderHolder> tree_type;
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(set_impl)

  typedef tree_type implementation_defined;

 public:
  typedef typename implementation_defined::value_type               value_type;
  typedef typename implementation_defined::key_type                 key_type;
  typedef typename implementation_defined::key_of_value             key_of_value;
  typedef typename implementation_defined::value_traits             value_traits;
  typedef typename implementation_defined::pointer                  pointer;
  typedef typename implementation_defined::const_pointer            const_pointer;
  typedef typename implementation_defined::reference                reference;
  typedef typename implementation_defined::const_reference          const_reference;
  typedef typename implementation_defined::difference_type          difference_type;
  typedef typename implementation_defined::size_type                size_type;
  typedef typename implementation_defined::value_compare            value_compare;
  typedef typename implementation_defined::key_compare              key_compare;
  typedef typename implementation_defined::iterator                 iterator;
  typedef typename implementation_defined::const_iterator           const_iterator;
  typedef typename implementation_defined::reverse_iterator         reverse_iterator;
  typedef typename implementation_defined::const_reverse_iterator   const_reverse_iterator;
  typedef typename implementation_defined::insert_commit_data       insert_commit_data;
  typedef typename implementation_defined::node_traits              node_traits;
  typedef typename implementation_defined::node                     node;
  typedef typename implementation_defined::node_ptr                 node_ptr;
  typedef typename implementation_defined::const_node_ptr           const_node_ptr;
  typedef typename implementation_defined::node_algorithms          node_algorithms;

  static const bool constant_time_size = tree_type::constant_time_size;

 public:

  set_impl()
      :  tree_type()
  {}

  explicit set_impl( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  tree_type(cmp, v_traits)
  {}

  template<class Iterator>
  set_impl( Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      : tree_type(true, b, e, cmp, v_traits)
  {}

  set_impl(AUTODDS_RV_REF(set_impl) x)
      :  tree_type(AUTODDS_MOVE_BASE(tree_type, x))
  {}

  set_impl& operator=(AUTODDS_RV_REF(set_impl) x)
  {  return static_cast<set_impl&>(tree_type::operator=(AUTODDS_MOVE_BASE(tree_type, x))); }

  using tree_type::clone_from;

  template <class Cloner, class Disposer>
  void clone_from(AUTODDS_RV_REF(set_impl) src, Cloner cloner, Disposer disposer)
  {  tree_type::clone_from(AUTODDS_MOVE_BASE(tree_type, src), cloner, disposer);  }

  std::pair<iterator, bool> insert(reference value)
  {  return tree_type::insert_unique(value);  }

  iterator insert(const_iterator hint, reference value)
  {  return tree_type::insert_unique(hint, value);  }

  std::pair<iterator, bool> insert_check
      (const key_type &key, insert_commit_data &commit_data)
  {  return tree_type::insert_unique_check(key, commit_data); }

  std::pair<iterator, bool> insert_check
      (const_iterator hint, const key_type &key
          ,insert_commit_data &commit_data)
  {  return tree_type::insert_unique_check(hint, key, commit_data); }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator, bool> insert_check
      (const KeyType &key, KeyTypeKeyCompare comp, insert_commit_data &commit_data)
  {  return tree_type::insert_unique_check(key, comp, commit_data); }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator, bool> insert_check
      (const_iterator hint, const KeyType &key
          ,KeyTypeKeyCompare comp, insert_commit_data &commit_data)
  {  return tree_type::insert_unique_check(hint, key, comp, commit_data); }

  template<class Iterator>
  void insert(Iterator b, Iterator e)
  {  tree_type::insert_unique(b, e);  }

  iterator insert_commit(reference value, const insert_commit_data &commit_data) AUTODDS_NOEXCEPT
  {  return tree_type::insert_unique_commit(value, commit_data);  }

  size_type count(const key_type &key) const
  {  return static_cast<size_type>(this->tree_type::find(key) != this->tree_type::cend()); }

  template<class KeyType, class KeyTypeKeyCompare>
  size_type count(const KeyType& key, KeyTypeKeyCompare comp) const
  {  return static_cast<size_type>(this->tree_type::find(key, comp) != this->tree_type::cend()); }

  std::pair<iterator,iterator> equal_range(const key_type &key)
  {  return this->tree_type::lower_bound_range(key); }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<iterator,iterator> equal_range(const KeyType& key, KeyTypeKeyCompare comp)
  {  return this->tree_type::equal_range(key, comp); }

  std::pair<const_iterator, const_iterator>
  equal_range(const key_type &key) const
  {  return this->tree_type::lower_bound_range(key); }

  template<class KeyType, class KeyTypeKeyCompare>
  std::pair<const_iterator, const_iterator>
  equal_range(const KeyType& key, KeyTypeKeyCompare comp) const
  {  return this->tree_type::equal_range(key, comp); }

  template<class Compare2>
  void merge(set_impl<ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, HeaderHolder> &source)
  {  return tree_type::merge_unique(source);  }


  template<class Compare2>
  void merge(multiset_impl<ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, HeaderHolder> &source)
  {  return tree_type::merge_unique(source);  }

};

template<class T, class ...Options>
struct make_set
{
  typedef typename pack_options<rbtree_defaults, Options...>::type packed_options;

  typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type value_traits;

  typedef set_impl
      < value_traits
          , typename packed_options::key_of_value
          , typename packed_options::compare
          , typename packed_options::size_type
          , packed_options::constant_time_size
          , typename packed_options::header_holder_type
      > implementation_defined;
  typedef implementation_defined type;
};

template<class T, class ...Options>
class set : public make_set<T, Options...>::type
{
  typedef typename make_set<T, Options...>::type Base;

 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(set)
 public:
  typedef typename Base::key_compare        key_compare;
  typedef typename Base::value_traits       value_traits;
  typedef typename Base::iterator           iterator;
  typedef typename Base::const_iterator     const_iterator;

  //Assert if passed value traits are compatible with the type
  AUTODDS_STATIC_ASSERT((detail::is_same<typename value_traits::value_type, T>::value));

  AUTODDS_INTRUSIVE_FORCEINLINE set()
      :  Base()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit set( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
  {}

  template<class Iterator>
  AUTODDS_INTRUSIVE_FORCEINLINE set( Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      :  Base(b, e, cmp, v_traits)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE set(AUTODDS_RV_REF(set) x)
      :  Base(AUTODDS_MOVE_BASE(Base, x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE set& operator=(AUTODDS_RV_REF(set) x)
  {  return static_cast<set &>(this->Base::operator=(AUTODDS_MOVE_BASE(Base, x)));  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(const set &src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(src, cloner, disposer);  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(AUTODDS_RV_REF(set) src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(AUTODDS_MOVE_BASE(Base, src), cloner, disposer);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static set &container_from_end_iterator(iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<set &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const set &container_from_end_iterator(const_iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<const set &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static set &container_from_iterator(iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<set &>(Base::container_from_iterator(it));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const set &container_from_iterator(const_iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<const set &>(Base::container_from_iterator(it));   }
};

template<class ValueTraits, class VoidOrKeyOfValue, class Compare, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
class multiset_impl :
    public bstree_impl<ValueTraits, VoidOrKeyOfValue, Compare, SizeType, ConstantTimeSize, RbTreeAlgorithms, HeaderHolder>
{
  typedef bstree_impl<ValueTraits, VoidOrKeyOfValue, Compare, SizeType, ConstantTimeSize, RbTreeAlgorithms, HeaderHolder> tree_type;
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(multiset_impl)
  typedef tree_type implementation_defined;

 public:
  typedef typename implementation_defined::value_type               value_type;
  typedef typename implementation_defined::key_type                 key_type;
  typedef typename implementation_defined::key_of_value             key_of_value;
  typedef typename implementation_defined::value_traits             value_traits;
  typedef typename implementation_defined::pointer                  pointer;
  typedef typename implementation_defined::const_pointer            const_pointer;
  typedef typename implementation_defined::reference                reference;
  typedef typename implementation_defined::const_reference          const_reference;
  typedef typename implementation_defined::difference_type          difference_type;
  typedef typename implementation_defined::size_type                size_type;
  typedef typename implementation_defined::value_compare            value_compare;
  typedef typename implementation_defined::key_compare              key_compare;
  typedef typename implementation_defined::iterator                 iterator;
  typedef typename implementation_defined::const_iterator           const_iterator;
  typedef typename implementation_defined::reverse_iterator         reverse_iterator;
  typedef typename implementation_defined::const_reverse_iterator   const_reverse_iterator;
  typedef typename implementation_defined::insert_commit_data       insert_commit_data;
  typedef typename implementation_defined::node_traits              node_traits;
  typedef typename implementation_defined::node                     node;
  typedef typename implementation_defined::node_ptr                 node_ptr;
  typedef typename implementation_defined::const_node_ptr           const_node_ptr;
  typedef typename implementation_defined::node_algorithms          node_algorithms;

  static const bool constant_time_size = tree_type::constant_time_size;

  multiset_impl()
      :  tree_type()
  {}

  explicit multiset_impl( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  tree_type(cmp, v_traits)
  {}

  template<class Iterator>
  multiset_impl( Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      : tree_type(false, b, e, cmp, v_traits)
  {}

  multiset_impl(AUTODDS_RV_REF(multiset_impl) x)
      :  tree_type(AUTODDS_MOVE_BASE(tree_type, x))
  {}

  multiset_impl& operator=(AUTODDS_RV_REF(multiset_impl) x)
  {  return static_cast<multiset_impl&>(tree_type::operator=(AUTODDS_MOVE_BASE(tree_type, x))); }

  using tree_type::clone_from;

  template <class Cloner, class Disposer>
  void clone_from(AUTODDS_RV_REF(multiset_impl) src, Cloner cloner, Disposer disposer)
  {  tree_type::clone_from(AUTODDS_MOVE_BASE(tree_type, src), cloner, disposer);  }

  iterator insert(reference value)
  {  return tree_type::insert_equal(value);  }

  iterator insert(const_iterator hint, reference value)
  {  return tree_type::insert_equal(hint, value);  }

  template<class Iterator>
  void insert(Iterator b, Iterator e)
  {  tree_type::insert_equal(b, e);  }

  template<class Compare2>
  void merge(multiset_impl<ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, HeaderHolder> &source)
  {  return tree_type::merge_equal(source);  }

  template<class Compare2>
  void merge(set_impl<ValueTraits, VoidOrKeyOfValue, Compare2, SizeType, ConstantTimeSize, HeaderHolder> &source)
  {  return tree_type::merge_equal(source);  }

};

template<class T, class ...Options>
struct make_multiset
{
  typedef typename pack_options<T, Options...>::type  packed_options;

  typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type value_traits;

  typedef multiset_impl
      < value_traits
          , typename packed_options::key_of_value
          , typename packed_options::compare
          , typename packed_options::size_type
          , packed_options::constant_time_size
          , typename packed_options::header_holder_type
      > implementation_defined;
  /// @endcond
  typedef implementation_defined type;
};

template<class T, class ...Options>
class multiset : public make_multiset<T, Options...>::type
{
  typedef typename make_multiset<T, Options...>::type Base;

  AUTODDS_MOVABLE_BUT_NOT_COPYABLE(multiset)

 public:
  typedef typename Base::key_compare        key_compare;
  typedef typename Base::value_traits       value_traits;
  typedef typename Base::iterator           iterator;
  typedef typename Base::const_iterator     const_iterator;

  //Assert if passed value traits are compatible with the type
  AUTODDS_STATIC_ASSERT((detail::is_same<typename value_traits::value_type, T>::value));

  AUTODDS_INTRUSIVE_FORCEINLINE multiset()
      :  Base()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit multiset( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
  {}

  template<class Iterator>
  AUTODDS_INTRUSIVE_FORCEINLINE multiset( Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      :  Base(b, e, cmp, v_traits)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE multiset(AUTODDS_RV_REF(multiset) x)
  :  Base(AUTODDS_MOVE_BASE(Base, x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE multiset& operator=(AUTODDS_RV_REF(multiset) x)
  {  return static_cast<multiset &>(this->Base::operator=(AUTODDS_MOVE_BASE(Base, x)));  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(const multiset &src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(src, cloner, disposer);  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(AUTODDS_RV_REF(multiset) src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(AUTODDS_MOVE_BASE(Base, src), cloner, disposer);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static multiset &container_from_end_iterator(iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<multiset &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const multiset &container_from_end_iterator(const_iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<const multiset &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static multiset &container_from_iterator(iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<multiset &>(Base::container_from_iterator(it));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const multiset &container_from_iterator(const_iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<const multiset &>(Base::container_from_iterator(it));   }
  
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_SET_HPP_