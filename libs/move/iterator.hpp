//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_ITERATOR_HPP_
#define AUTODDS_LIBS_MOVE_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/iterator_traits.hpp"
#include "libs/move/utility_core.hpp"

namespace autodds {
namespace libs {

//! Class template move_iterator is an iterator adaptor with the same behavior
//! as the underlying iterator except that its dereference operator implicitly
//! converts the value returned by the underlying iterator's dereference operator
//! to an rvalue reference. Some generic algorithms can be called with move
//! iterators to replace copying with moving.
template <typename Iterator>
class move_iterator
{
 public:
  typedef Iterator                                                      iterator_type;
  typedef typename movelib::iterator_traits<iterator_type>::value_type  value_type;
  typedef value_type &&                                                 reference;
  typedef Iterator                                                      pointer;
  typedef typename movelib::iterator_traits<iterator_type>::difference_type    difference_type;
  typedef typename movelib::iterator_traits<iterator_type>::iterator_category  iterator_category;

  AUTODDS_MOVE_FORCEINLINE move_iterator()
      : m_iter()
  {}

  AUTODDS_MOVE_FORCEINLINE move_iterator(const Iterator& iter)
      : m_iter(iter)
  {}

  template<class U>
  AUTODDS_MOVE_FORCEINLINE move_iterator(const move_iterator<U>& other)
      : m_iter(other.m_iter)
  {}

  AUTODDS_MOVE_FORCEINLINE reference operator * () const
  { return autodds::libs::move(*m_iter); }

  AUTODDS_MOVE_FORCEINLINE pointer operator -> () const
  { return m_iter; }

  AUTODDS_MOVE_FORCEINLINE move_iterator& operator ++ ()
  {
    ++m_iter;
    return *this;
  }

  AUTODDS_MOVE_FORCEINLINE move_iterator<iterator_type> operator ++ (int)
  {
    move_iterator<iterator_type> tmp(*this);
    ++(*this);
    return tmp;
  }

  AUTODDS_MOVE_FORCEINLINE move_iterator& operator -- ()
  {
    --m_iter;
    return *this;
  }

  AUTODDS_MOVE_FORCEINLINE move_iterator<iterator_type> operator -- (int)
  {
    move_iterator<iterator_type> tmp(*this);
    --(*this);
    return tmp;
  }

  move_iterator<iterator_type>  operator + (difference_type n) const
  {  return move_iterator<iterator_type>(m_iter + n);  }

  AUTODDS_MOVE_FORCEINLINE move_iterator& operator += (difference_type n)
  {
    m_iter += n;
    return *this;
  }

  AUTODDS_MOVE_FORCEINLINE move_iterator<iterator_type>  operator- (difference_type n) const
  {  return move_iterator<iterator_type>(m_iter - n);  }

  AUTODDS_MOVE_FORCEINLINE move_iterator& operator -= (difference_type n)
  {  m_iter -= n; return *this;   }

  AUTODDS_MOVE_FORCEINLINE reference operator [] (difference_type idx) const
  {
    return autodds::libs::move(m_iter[idx]);
  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator == (const move_iterator& x, const move_iterator& y)
  { return x.m_iter == y.m_iter; }

  AUTODDS_MOVE_FORCEINLINE friend bool operator != (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter != y.m_iter;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator < (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter < y.m_iter;   }

  AUTODDS_MOVE_FORCEINLINE friend bool operator <= (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter <= y.m_iter;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator > (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter > y.m_iter;  }

  AUTODDS_MOVE_FORCEINLINE friend bool operator >= (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter >= y.m_iter;  }

  AUTODDS_MOVE_FORCEINLINE friend difference_type operator - (const move_iterator& x, const move_iterator& y)
  {  return x.m_iter - y.m_iter;   }

  AUTODDS_MOVE_FORCEINLINE friend move_iterator operator + (difference_type n, const move_iterator& x)
  {  return move_iterator(x.m_iter + n);   }

 private:
  Iterator m_iter;
};

namespace move_detail {

template <typename T>
struct is_move_iterator
{
  static const bool value = false;
};

template <typename T>
struct is_move_iterator<autodds::libs::move_iterator<T>>
{
  static const bool value = true;
};

} // namespace move_detail

template <typename Iter>
AUTODDS_MOVE_FORCEINLINE move_iterator<Iter> make_move_iterator(const Iter& it)
{ return move_iterator<Iter>(it); }

//! A move insert iterator that move constructs elements at the
//! back of a container
template <typename Container>
class back_move_insert_iterator
{
  Container* container_m;
 public:

  typedef Container                                   container_type;
  typedef typename Container::value_type              value_type;
  typedef typename Container::reference               reference;
  typedef typename Container::pointer                 pointer;
  typedef typename Container::difference_type         difference_type;
  typedef std::output_iterator_tag                    iterator_category;

  explicit back_move_insert_iterator(Container& x) : container_m(x)
  {}

  back_move_insert_iterator& operator = (reference x)
  {
    container_m->push_back(::autodds::libs::move(x));
    return *this;
  }

  back_move_insert_iterator& operator = ( AUTODDS_RV_REF(value_type) x)
  {
    reference rx = x;
    return this->operator=(rx);
  }
};

template <typename C>
inline back_move_insert_iterator<C> back_move_inserter(C& x)
{
  return back_move_insert_iterator<C>(x);
}

//! A move insert iterator that move constructs elements int the
//! front of a container
template <typename Container>
class front_move_insert_iterator
{
  Container* container_m;
 public:
  typedef Container                              container_type;
  typedef typename Container::value_type         value_type;
  typedef typename Container::reference          reference;
  typedef typename Container::pointer            pointer;
  typedef typename Container::difference_type    difference_type;
  typedef std::output_iterator_tag               iterator_category;

  explicit front_move_insert_iterator(Container& x) : container_m(x)
  {}

  front_move_insert_iterator& operator = (reference x)
  {
    container_m->push_front(autodds::libs::move(x));
    return *this;
  }

  front_move_insert_iterator& operator=(AUTODDS_RV_REF(value_type) x)
  {
    reference rx = x;
    return this->operator=(rx);
  }

  front_move_insert_iterator& operator*()     { return *this; }
  front_move_insert_iterator& operator++()    { return *this; }
  front_move_insert_iterator& operator++(int) { return *this; }
};

template <typename C>
inline front_move_insert_iterator<C> front_move_inserter(C& c)
{
  return front_move_insert_iterator<C>(c);
}

//                         insert_move_iterator
template <typename Container>
class move_insert_iterator
{
  Container* container_m;
  typename Container::iterator pos_;

 public:
  typedef Container                              container_type;
  typedef typename Container::value_type         value_type;
  typedef typename Container::reference          reference;
  typedef typename Container::pointer            pointer;
  typedef typename Container::difference_type    difference_type;
  typedef std::output_iterator_tag               iterator_category;

  explicit move_insert_iterator(Container& x, typename Container::iterator pos)
      : container_m(&x), pos_(pos)
  {}

  move_insert_iterator& operator=(reference x)
  {
    pos_ = container_m->insert(pos_, ::autodds::libs::move(x));
    ++pos_;
    return *this;
  }

  move_insert_iterator& operator=(AUTODDS_RV_REF(value_type) x)
  {  reference rx = x; return this->operator=(rx);  }

  move_insert_iterator& operator*()     { return *this; }
  move_insert_iterator& operator++()    { return *this; }
  move_insert_iterator& operator++(int) { return *this; }
};

template <typename C> // C models Container
inline move_insert_iterator<C> move_inserter(C& x, typename C::iterator it)
{
  return move_insert_iterator<C>(x, it);
}

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_ITERATOR_HPP_