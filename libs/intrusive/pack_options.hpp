//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_PACK_OPTIONS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_PACK_OPTIONS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

//index_tuple
template<std::size_t... Indexes>
struct index_tuple{};

//build_number_seq
template<std::size_t Num, typename Tuple = index_tuple<> >
struct build_number_seq;

template<std::size_t Num, std::size_t... Indexes>
struct build_number_seq<Num, index_tuple<Indexes...> >
   : build_number_seq<Num - 1, index_tuple<Indexes..., sizeof...(Indexes)> >
{};

template<std::size_t... Indexes>
struct build_number_seq<0, index_tuple<Indexes...> >
{  typedef index_tuple<Indexes...> type;  };

template<class ...Types>
struct typelist
{};

//invert_typelist
template<class T>
struct invert_typelist;

template<std::size_t I, typename Tuple>
struct typelist_element;

template<std::size_t I, typename Head, typename... Tail>
struct typelist_element<I, typelist<Head, Tail...> >
{
   typedef typename typelist_element<I-1, typelist<Tail...> >::type type;
};

template<typename Head, typename... Tail>
struct typelist_element<0, typelist<Head, Tail...> >
{
   typedef Head type;
};

template<std::size_t ...Ints, class ...Types>
typelist<typename typelist_element<(sizeof...(Types) - 1) - Ints, typelist<Types...> >::type...>
   inverted_typelist(index_tuple<Ints...>, typelist<Types...>)
{
   return typelist<typename typelist_element<(sizeof...(Types) - 1) - Ints, typelist<Types...> >::type...>();
}

//sizeof_typelist
template<class Typelist>
struct sizeof_typelist;

template<class ...Types>
struct sizeof_typelist< typelist<Types...> >
{
   static const std::size_t value = sizeof...(Types);
};

//invert_typelist_impl
template<class Typelist, class Indexes>
struct invert_typelist_impl;


template<class Typelist, std::size_t ...Ints>
struct invert_typelist_impl< Typelist, index_tuple<Ints...> >
{
   static const std::size_t last_idx = sizeof_typelist<Typelist>::value - 1;
   typedef typelist
      <typename typelist_element<last_idx - Ints, Typelist>::type...> type;
};

template<class Typelist, std::size_t Int>
struct invert_typelist_impl< Typelist, index_tuple<Int> >
{
   typedef Typelist type;
};

template<class Typelist>
struct invert_typelist_impl< Typelist, index_tuple<> >
{
   typedef Typelist type;
};

//invert_typelist
template<class Typelist>
struct invert_typelist;

template<class ...Types>
struct invert_typelist< typelist<Types...> >
{
   typedef typelist<Types...> typelist_t;
   typedef typename build_number_seq<sizeof...(Types)>::type indexes_t;
   typedef typename invert_typelist_impl<typelist_t, indexes_t>::type type;
};

//Do pack
template<class Typelist>
struct do_pack;

template<>
struct do_pack<typelist<> >;

template<class Prev>
struct do_pack<typelist<Prev> >
{
   typedef Prev type;
};

template<class Prev, class Last>
struct do_pack<typelist<Prev, Last> >
{
   typedef typename Prev::template pack<Last> type;
};

template<class ...Others>
struct do_pack<typelist<void, Others...> >
{
   typedef typename do_pack<typelist<Others...> >::type type;
};

template<class Prev, class ...Others>
struct do_pack<typelist<Prev, Others...> >
{
   typedef typename Prev::template pack
      <typename do_pack<typelist<Others...> >::type> type;
};


template<class DefaultOptions, class ...Options>
struct pack_options
{
   typedef typelist<DefaultOptions, Options...> typelist_t;
   typedef typename invert_typelist<typelist_t>::type inverted_typelist;
   typedef typename do_pack<inverted_typelist>::type type;
};


#define AUTODDS_INTRUSIVE_OPTION_TYPE(OPTION_NAME, TYPE, TYPEDEF_EXPR, TYPEDEF_NAME) \
template< class TYPE> \
struct OPTION_NAME \
{ \
   template<class Base> \
   struct pack : Base \
   { \
      typedef TYPEDEF_EXPR TYPEDEF_NAME; \
   }; \
}; \
//

#define AUTODDS_INTRUSIVE_OPTION_CONSTANT(OPTION_NAME, TYPE, VALUE, CONSTANT_NAME) \
template< TYPE VALUE> \
struct OPTION_NAME \
{ \
   static const TYPE value = VALUE; \
 \
   template<class Base> \
   struct pack : Base \
   { \
      static const TYPE CONSTANT_NAME = VALUE; \
   }; \
}; \
//

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_PACK_OPTIONS_HPP_