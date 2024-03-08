//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/ebo_functor_holder.hpp"
#include "libs/intrusive/pointer_traits.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

using autodds::intrusive::detail::disable_if_c;
using autodds::intrusive::detail::is_same;

template<typename From, typename ValuePtr>
struct disable_if_smartref_to : public disable_if_c<
    is_same<From, typename autodds::intrusive::pointer_traits<ValuePtr>::reference>::value || 
    is_same<From, typename autodds::intrusive::pointer_traits<typename pointer_rebind<ValuePtr,
    const typename autodds::movelib::pointer_element<ValuePtr>::type>::type>::reference>::value>
{};

//This function object takes a KeyCompare function object
//and compares values that contains keys using KeyOfValue
template< class ValuePtr, class KeyCompare, class KeyOfValue, class Ret = bool
    , bool = is_same<typename autodds::movelib::pointer_element<ValuePtr>::type, 
    typename KeyOfValue::type>::value >
struct tree_value_compare : public detail::ebo_functor_holder<KeyCompare>
{
  typedef typename
  autodds::movelib::pointer_element<ValuePtr>::type  value_type;
  typedef KeyCompare                                 key_compare;
  typedef KeyOfValue                                 key_of_value;
  typedef typename KeyOfValue::type                  key_type;
  typedef detail::ebo_functor_holder<KeyCompare>     base_t;

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare()
      :  base_t()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit tree_value_compare(const key_compare &kcomp)
      :  base_t(kcomp)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare (const tree_value_compare &x)
      :  base_t(x.base_t::get())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare &operator=(const tree_value_compare &x)
  {  this->base_t::get() = x.base_t::get();   return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare &operator=(const key_compare &x)
  {  this->base_t::get() = x;   return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE const key_compare &key_comp() const
  {  return static_cast<const key_compare &>(*this);  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const key_type &key) const
  {  return this->key_comp()(key);   }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const value_type &value) const
  {  return this->key_comp()(KeyOfValue()(value));  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const U &nonkey
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(nonkey);  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const key_type &key1, const key_type &key2) const
  {  return this->key_comp()(key1, key2);  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const value_type &value1, const value_type &value2) const
  {  return this->key_comp()(KeyOfValue()(value1), KeyOfValue()(value2));  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const key_type &key1, const value_type &value2) const
  {  return this->key_comp()(key1, KeyOfValue()(value2));  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const value_type &value1, const key_type &key2) const
  {  return this->key_comp()(KeyOfValue()(value1), key2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const key_type &key1, const U &nonkey2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(key1, nonkey2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const U &nonkey1, const key_type &key2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(nonkey1, key2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const value_type &value1, const U &nonvalue2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(KeyOfValue()(value1), nonvalue2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const U &nonvalue1, const value_type &value2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(nonvalue1, KeyOfValue()(value2));  }
  
};


template<class ValuePtr, class KeyCompare, class KeyOfValue, class Ret>
struct tree_value_compare<ValuePtr, KeyCompare, KeyOfValue, Ret, true>
    :  public detail::ebo_functor_holder<KeyCompare>
{
  typedef typename
  autodds::movelib::pointer_element<ValuePtr>::type value_type;
  typedef KeyCompare                                 key_compare;
  typedef KeyOfValue                                 key_of_value;
  typedef typename KeyOfValue::type                  key_type;

  typedef detail::ebo_functor_holder<KeyCompare>     base_t;


  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare()
      :  base_t()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit tree_value_compare(const key_compare &kcomp)
      :  base_t(kcomp)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare (const tree_value_compare &x)
      :  base_t(x.base_t::get())
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare &operator=(const tree_value_compare &x)
  {  this->base_t::get() = x.base_t::get();   return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE tree_value_compare &operator=(const key_compare &x)
  {  this->base_t::get() = x;   return *this;  }

  AUTODDS_INTRUSIVE_FORCEINLINE const key_compare &key_comp() const
  {  return static_cast<const key_compare &>(*this);  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const key_type &key) const
  {  return this->key_comp()(key);   }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const U &nonkey
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(nonkey);  }

  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const key_type &key1, const key_type &key2) const
  {  return this->key_comp()(key1, key2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()( const key_type &key1, const U &nonkey2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(key1, nonkey2);  }

  template<class U>
  AUTODDS_INTRUSIVE_FORCEINLINE Ret operator()(const U &nonkey1, const key_type &key2
      , typename disable_if_smartref_to<U, ValuePtr>::type* = 0) const
  {  return this->key_comp()(nonkey1, key2);  }
};

} // namespace intrusive
} // namespace libs 
} // namespace intrusive

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP_