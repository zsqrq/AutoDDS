//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_TRANSFORM_ITERATOR_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_TRANSFORM_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/container/detail/type_traits.hpp"
#include "libs/container/detail/iterator.hpp"

namespace autodds {
namespace libs {
namespace container {

template <typename PseudoReference>
struct operator_arrow_proxy
{
  AUTODDS_CONTAINER_FORCEINLINE operator_arrow_proxy(const PseudoReference& px)
      : m_value(px)
  {}

  typedef PseudoReference element_type;

  AUTODDS_CONTAINER_FORCEINLINE PseudoReference* operator->() const { return &m_value; }

  mutable PseudoReference m_value;
};

template <typename T>
struct operator_arrow_proxy<T&>
{
  AUTODDS_CONTAINER_FORCEINLINE operator_arrow_proxy(T& px)
      : m_value(px)
  {}

  typedef T element_type;

  AUTODDS_CONTAINER_FORCEINLINE T* operator -> () const
  { return const_cast<T*>(&m_value); }

  T& m_value;
};

template <typename Iterator, typename UnaryFunction>
class transform_iterator :
    public UnaryFunction,
    public autodds::libs::container::iterator
        < typename Iterator::iterator_category,
          typename dtl::remove_reference<typename UnaryFunction::result_type>::type,
          typename Iterator::difference_type,
          operator_arrow_proxy<typename UnaryFunction::result_type>,
          typename UnaryFunction::result_type>
{
 public:
  AUTODDS_CONTAINER_FORCEINLINE explicit transform_iterator(const Iterator& it, const UnaryFunction& f = UnaryFunction())
      : UnaryFunction(f), m_it(it)
  {}

  AUTODDS_CONTAINER_FORCEINLINE explicit transform_iterator()
      :  UnaryFunction(), m_it()
  {}

  AUTODDS_CONTAINER_FORCEINLINE transform_iterator& operator++()
  { increment();   return *this;   }

  AUTODDS_CONTAINER_FORCEINLINE transform_iterator operator++(int)
  {
    transform_iterator result (*this);
    increment();
    return result;
  }

  AUTODDS_CONTAINER_FORCEINLINE friend bool operator== (const transform_iterator& i, const transform_iterator& i2)
  { return i.equal(i2); }

  AUTODDS_CONTAINER_FORCEINLINE friend bool operator!= (const transform_iterator& i, const transform_iterator& i2)
  { return !(i == i2); }

  AUTODDS_CONTAINER_FORCEINLINE friend typename Iterator::difference_type operator- (const transform_iterator& i, const transform_iterator& i2)
  { return i2.distance_to(i); }

  //Arithmetic
  AUTODDS_CONTAINER_FORCEINLINE transform_iterator& operator+=(typename Iterator::difference_type off)
  {  this->advance(off); return *this;   }

  AUTODDS_CONTAINER_FORCEINLINE transform_iterator operator+(typename Iterator::difference_type off) const
  {
    transform_iterator other(*this);
    other.advance(off);
    return other;
  }

  AUTODDS_CONTAINER_FORCEINLINE friend transform_iterator operator+(typename Iterator::difference_type off, const transform_iterator& right)
  {  return right + off; }

  AUTODDS_CONTAINER_FORCEINLINE transform_iterator& operator-=(typename Iterator::difference_type off)
  {  this->advance(-off); return *this;   }

  AUTODDS_CONTAINER_FORCEINLINE transform_iterator operator-(typename Iterator::difference_type off) const
  {  return *this + (-off);  }

  AUTODDS_CONTAINER_FORCEINLINE typename UnaryFunction::result_type operator*() const
  { return dereference(); }

  AUTODDS_CONTAINER_FORCEINLINE operator_arrow_proxy<typename UnaryFunction::result_type>
  operator->() const
  { return operator_arrow_proxy<typename UnaryFunction::result_type>(dereference());  }

  AUTODDS_CONTAINER_FORCEINLINE Iterator & base()
  {  return m_it;   }

  AUTODDS_CONTAINER_FORCEINLINE const Iterator & base() const
  {  return m_it;   }

 private:
  Iterator m_it;

  AUTODDS_CONTAINER_FORCEINLINE void increment()
  { ++m_it; }

  AUTODDS_CONTAINER_FORCEINLINE void decrement()
  { --m_it; }

  AUTODDS_CONTAINER_FORCEINLINE bool equal(const transform_iterator &other) const
  {  return m_it == other.m_it;   }

  AUTODDS_CONTAINER_FORCEINLINE bool less(const transform_iterator &other) const
  {  return other.m_it < m_it;   }

  AUTODDS_CONTAINER_FORCEINLINE typename UnaryFunction::result_type dereference() const
  { return UnaryFunction::operator()(*m_it); }

  AUTODDS_CONTAINER_FORCEINLINE void advance(typename Iterator::difference_type n)
  {  autodds::libs::container::iterator_advance(m_it, n); }

  AUTODDS_CONTAINER_FORCEINLINE typename Iterator::difference_type distance_to(const transform_iterator &other)const
  {  return autodds::libs::container::iterator_distance(other.m_it, m_it); }
};

template <typename Iterator, typename UnaryFunc>
AUTODDS_CONTAINER_FORCEINLINE transform_iterator<Iterator, UnaryFunc>
make_transform_iterator(Iterator it, UnaryFunc fun)
{
  return transform_iterator<Iterator, UnaryFunc>(it, fun);
}

} // namespace container
} // namespace libs
} // namespace autodds

#include "libs/container/detail/config_end.hpp"

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_TRANSFORM_ITERATOR_HPP_