//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_REVERSE_ITERATOR_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_REVERSE_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace movelib {


template<class I>
AUTODDS_MOVE_FORCEINLINE typename iterator_traits<I>::pointer iterator_arrow_result(const I &i)
{  return i.operator->();  }

template<class T>
AUTODDS_MOVE_FORCEINLINE T * iterator_arrow_result(T *p)
{  return p;   }

template<class It>
class reverse_iterator
{
 public:
  typedef typename autodds::libs::movelib::iterator_traits<It>::pointer             pointer;
  typedef typename autodds::libs::movelib::iterator_traits<It>::reference           reference;
  typedef typename autodds::libs::movelib::iterator_traits<It>::difference_type     difference_type;
  typedef typename autodds::libs::movelib::iterator_traits<It>::iterator_category   iterator_category;
  typedef typename autodds::libs::movelib::iterator_traits<It>::value_type          value_type;


  typedef It iterator_type;

  AUTODDS_MOVE_FORCEINLINE reverse_iterator()
      : m_current()  //Value initialization to achieve "null iterators" (N3644)
  {}

  AUTODDS_MOVE_FORCEINLINE explicit reverse_iterator(It r)
      : m_current(r)
  {}

  AUTODDS_MOVE_FORCEINLINE reverse_iterator(const reverse_iterator& r)
      : m_current(r.base())
  {}

  template<class OtherIt>
  AUTODDS_MOVE_FORCEINLINE
  reverse_iterator( const reverse_iterator<OtherIt>& r
      , typename autodds::libs::move_detail::enable_if_convertible<OtherIt, It>::type* =0
  )
      : m_current(r.base())
  {}

  AUTODDS_MOVE_FORCEINLINE reverse_iterator & operator=( const reverse_iterator& r)
  {  m_current = r.base();   return *this;  }

  template<class OtherIt>
  AUTODDS_MOVE_FORCEINLINE typename autodds::libs::move_detail::enable_if_convertible<OtherIt, It, reverse_iterator &>::type
  operator=( const reverse_iterator<OtherIt>& r)
  {  m_current = r.base();   return *this;  }

  AUTODDS_MOVE_FORCEINLINE It base() const
  {  return m_current;  }

  AUTODDS_MOVE_FORCEINLINE reference operator*() const
  {
    It temp(m_current);
    --temp;
    reference r = *temp;
    return r;
  }

  AUTODDS_MOVE_FORCEINLINE pointer operator->() const
  {
    It temp(m_current);
    --temp;
    return (iterator_arrow_result)(temp);
  }

  AUTODDS_MOVE_FORCEINLINE reference operator[](difference_type off) const
  {
    return this->m_current[difference_type(-off - 1)];
  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator& operator++()
  {
    --m_current;
    return *this;
  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator operator++(int)
  {
    reverse_iterator temp((*this));
    --m_current;
    return temp;
  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator& operator--()
  {
    ++m_current;
    return *this;
  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator operator--(int)
  {
    reverse_iterator temp((*this));
    ++m_current;
    return temp;
  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator==(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current == r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator!=(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current != r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator<(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current > r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator<=(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current >= r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator>(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current < r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator>=(const reverse_iterator& l, const reverse_iterator& r)
  {  return l.m_current <= r.m_current;  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator& operator+=(difference_type off)
  {  m_current -= off; return *this;  }

  AUTODDS_MOVE_FORCEINLINE reverse_iterator& operator-=(difference_type off)
  {  m_current += off; return *this;  }

  AUTODDS_MOVE_FORCEINLINE friend reverse_iterator operator+(reverse_iterator l, difference_type off)
  {  return (l += off);  }

  AUTODDS_MOVE_FORCEINLINE friend reverse_iterator operator+(difference_type off, reverse_iterator r)
  {  return (r += off);   }

  AUTODDS_MOVE_FORCEINLINE friend reverse_iterator operator-(reverse_iterator l, difference_type off)
  {  return (l-= off);  }

  AUTODDS_MOVE_FORCEINLINE friend difference_type operator-(const reverse_iterator& l, const reverse_iterator& r)
  {  return r.m_current - l.m_current;  }

 private:
  It m_current;   // the wrapped iterator
};

template< class Iterator >
AUTODDS_MOVE_FORCEINLINE reverse_iterator<Iterator> make_reverse_iterator( Iterator i )
{  return reverse_iterator<Iterator>(i);  }

} // namespace movelib
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_DETAIL_REVERSE_ITERATOR_HPP_