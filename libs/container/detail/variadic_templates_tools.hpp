//
// Created by wz on 24-3-5.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/container/detail/type_traits.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

template <typename... Values>
class tuple;

template <>
class tuple<> {};

template <typename Head, typename... Tail>
class tuple<Head, Tail...> : private tuple<Tail...>
{
  typedef tuple<Tail...> inherited;

 public:
  tuple() : inherited(), m_head()
  {}

  template<class U, class... Args>
  tuple(U&& u, Args&&... args)
      :inherited(::autodds::libs::forward<Args>(args)...), m_head(::autodds::libs::forward<U>(u))
  {}

  template <typename... Vals>
  tuple& operator =(const tuple<Vals...>& other)
  {
    m_head = other.head();
    tail() = other.tail();
    return this;
  }

  typename add_reference<Head>::type head()
  { return m_head; }

  typename add_reference<const Head>::type head() const
  { return m_head; }

  inherited & tail()
  { return *this; }

  const inherited& tail() const
  { return *this; }

 protected:
  Head m_head;
};

template <typename... Values>
tuple<Values&&...> forward_as_tuple_impl(Values&&... values)
{
  return tuple<Values&&...>(autodds::libs::forward<Values>(values)...);
}

template <int I, typename Tuple>
struct tuple_element;

template <int I, typename Head, typename... Tail>
struct tuple_element<I, tuple<Head, Tail...>>
{
  typedef typename tuple_element<I-1, tuple<Tail...>>::type type;
};

template <typename Head, typename... Tail>
struct tuple_element<0, tuple<Head, Tail...> >
{
  typedef Head type;
};

template <int I, typename Tuple>
class get_impl;

template <int I, typename Head, typename... Values>
class get_impl<I, tuple<Head, Values...>>
{
  typedef typename tuple_element<I-1, tuple<Values...> >::type   Element;
  typedef get_impl<I-1, tuple<Values...> >                       Next;

 public:
  typedef typename add_reference<Element>::type                  type;
  typedef typename add_const_reference<Element>::type            const_type;
  static type get(tuple<Head, Values...>& t)              { return Next::get(t.tail()); }
  static const_type get(const tuple<Head, Values...>& t)  { return Next::get(t.tail()); }
};

template <typename Head, typename... Values>
class get_impl<0, tuple<Head, Values...> >
{
 public:
  typedef typename add_reference<Head>::type         type;
  typedef typename add_const_reference<Head>::type   const_type;
  static type       get(tuple<Head, Values...>& t)      { return t.head(); }
  static const_type get(const tuple<Head, Values...>& t){ return t.head(); }
};

template<int I, typename... Values>
typename get_impl<I, tuple<Values...> >::type get(tuple<Values...>& t)
{  return get_impl<I, tuple<Values...> >::get(t);  }

template<int I, typename... Values>
typename get_impl<I, tuple<Values...> >::const_type get(const tuple<Values...>& t)
{  return get_impl<I, tuple<Values...> >::get(t);  }

// Builds an index_tuple<0, 1, 2, ..., Num-1>, that will
// be used to "unpack" into comma-separated values
// in a function call.
template <std::size_t...>
struct index_tuple
{
  typedef index_tuple type;
};

template <typename S1, typename S2>
struct concat_index_tuple;

template<std::size_t... I1, std::size_t... I2>
struct concat_index_tuple<index_tuple<I1...>, index_tuple<I2...>>
    : index_tuple<I1..., (sizeof...(I1)+I2)...>{};

template<std::size_t N>
struct build_number_seq;

template <std::size_t N>
struct build_number_seq :
    concat_index_tuple<typename build_number_seq<N/2>::type,
                       typename build_number_seq<N - N/2>::type>::type
{};

template<> struct build_number_seq<0> : index_tuple<>{};
template<> struct build_number_seq<1> : index_tuple<0>{};

} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds

#include "libs/container/detail/config_end.hpp"

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_