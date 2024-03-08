//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_RBTREE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_RBTREE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"
#include "libs/intrusive/set_hook.hpp"
#include "libs/intrusive/detail/rbtree_node.hpp"
#include "libs/intrusive/bstree.hpp"
#include "libs/intrusive/detail/tree_node.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/get_value_traits.hpp"
#include "libs/intrusive/rbtree_algorithms.hpp"
#include "libs/intrusive/link_mode.hpp"

#include "libs/move/utility_core.hpp"
#include "libs/common/static_assert.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

struct default_rbtree_hook_applier
{
  template <typename T>
  struct apply
  {
    typedef typename T::default_rbtree_hook type;
  };
};

template<>
struct is_default_hook_tag<default_rbtree_hook_applier>
{  static const bool value = true;  };

struct rbtree_defaults
    : bstree_defaults
{
  typedef default_rbtree_hook_applier proto_value_traits;
};

//! The class template rbtree is an intrusive red-black tree container, that
//! is used to construct intrusive set and multiset containers. The no-throw
//! guarantee holds only, if the key_compare object
//! doesn't throw.
//!
template<class ValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
class rbtree_impl :
    public bstree_impl<ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, SizeType, ConstantTimeSize, RbTreeAlgorithms, HeaderHolder>
{
 public:
  typedef ValueTraits                                               value_traits;
  /// @cond
  typedef bstree_impl< ValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, SizeType
      , ConstantTimeSize, RbTreeAlgorithms
      , HeaderHolder>                                tree_type;
  typedef tree_type                                                 implementation_defined;
  /// @endcond

  typedef typename implementation_defined::pointer                  pointer;
  typedef typename implementation_defined::const_pointer            const_pointer;
  typedef typename implementation_defined::value_type               value_type;
  typedef typename implementation_defined::key_type                 key_type;
  typedef typename implementation_defined::key_of_value             key_of_value;
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
  typedef typename implementation_defined::node_traits              node_traits;
  typedef typename implementation_defined::node                     node;
  typedef typename implementation_defined::node_ptr                 node_ptr;
  typedef typename implementation_defined::const_node_ptr           const_node_ptr;
  typedef typename implementation_defined::node_algorithms          node_algorithms;

  static const bool constant_time_size = implementation_defined::constant_time_size;

 private:

  //noncopyable
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(rbtree_impl)

 public:

  typedef typename implementation_defined::insert_commit_data insert_commit_data;

  rbtree_impl()
      :  tree_type()
  {}

  explicit rbtree_impl( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  tree_type(cmp, v_traits)
  {}

  template<class Iterator>
  rbtree_impl( bool unique, Iterator b, Iterator e
      , const key_compare &cmp     = key_compare()
      , const value_traits &v_traits = value_traits())
      : tree_type(unique, b, e, cmp, v_traits)
  {}

  rbtree_impl(AUTODDS_RV_REF(rbtree_impl) x)
      :  tree_type(AUTODDS_MOVE_BASE(tree_type, x))
  {}

  rbtree_impl& operator=(AUTODDS_RV_REF(rbtree_impl) x)
  {  return static_cast<rbtree_impl&>(tree_type::operator=(AUTODDS_MOVE_BASE(tree_type, x))); }

  using tree_type::clone_from;

  template <class Cloner, class Disposer>
  void clone_from(AUTODDS_RV_REF(rbtree_impl) src, Cloner cloner, Disposer disposer)
  {  tree_type::clone_from(AUTODDS_MOVE_BASE(tree_type, src), cloner, disposer);  }

};

template<class T, class ...Options>
struct make_rbtree
{
  typedef typename pack_options<rbtree_defaults, Options...>::type   packed_options;
  typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type value_traits;

  typedef rbtree_impl
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
class rbtree : public make_rbtree<T, Options...>::type
{
  typedef typename make_rbtree<T,Options...>::type  Base;

  AUTODDS_MOVABLE_BUT_NOT_COPYABLE(rbtree)

 public:
  typedef typename Base::key_compare        key_compare;
  typedef typename Base::value_traits       value_traits;
  typedef typename Base::iterator           iterator;
  typedef typename Base::const_iterator     const_iterator;
  typedef typename Base::reverse_iterator           reverse_iterator;
  typedef typename Base::const_reverse_iterator     const_reverse_iterator;

  OOST_STATIC_ASSERT((detail::is_same<typename value_traits::value_type, T>::value));

  AUTODDS_INTRUSIVE_FORCEINLINE rbtree()
      :  Base()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit rbtree( const key_compare &cmp, const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
  {}

  template<class Iterator>
  AUTODDS_INTRUSIVE_FORCEINLINE rbtree( bool unique, Iterator b, Iterator e
      , const key_compare &cmp = key_compare()
      , const value_traits &v_traits = value_traits())
      :  Base(unique, b, e, cmp, v_traits)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE rbtree(AUTODDS_RV_REF(rbtree) x)
  :  Base(AUTODDS_MOVE_BASE(Base, x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE rbtree& operator=(AUTODDS_RV_REF(rbtree) x)
  {  return static_cast<rbtree &>(this->Base::operator=(AUTODDS_MOVE_BASE(Base, x)));  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(const rbtree &src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(src, cloner, disposer);  }

  template <class Cloner, class Disposer>
  AUTODDS_INTRUSIVE_FORCEINLINE void clone_from(AUTODDS_RV_REF(rbtree) src, Cloner cloner, Disposer disposer)
  {  Base::clone_from(AUTODDS_MOVE_BASE(Base, src), cloner, disposer);  }

  AUTODDS_INTRUSIVE_FORCEINLINE static rbtree &container_from_end_iterator(iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<rbtree &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const rbtree &container_from_end_iterator(const_iterator end_iterator) AUTODDS_NOEXCEPT
  {  return static_cast<const rbtree &>(Base::container_from_end_iterator(end_iterator));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static rbtree &container_from_iterator(iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<rbtree &>(Base::container_from_iterator(it));   }

  AUTODDS_INTRUSIVE_FORCEINLINE static const rbtree &container_from_iterator(const_iterator it) AUTODDS_NOEXCEPT
  {  return static_cast<const rbtree &>(Base::container_from_iterator(it));   }
  
};

} // namespace intrusive
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_INTRUSIVE_RBTREE_HPP_