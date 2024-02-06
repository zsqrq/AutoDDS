//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_POINTER_REBIND_HPP_
#define AUTODDS_LIBS_INTRUSIVE_POINTER_REBIND_HPP_

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_WORKAROUND_HPP_
#include "libs/intrusive/detail/workaround.hpp"
#endif
#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {

//struct pointer_rebind_mode
template <typename Ptr, typename U>
struct pointer_has_rebind
{
  template <typename V> struct any
  {  any(const V&) { } };

  template <typename X>
  static char test(int, typename X::template rebind<U>*);

  template <typename X>
  static int test(any<int>, void*);

  static const bool value = (1 == sizeof(test<Ptr>(0, 0)));
};

template <typename Ptr, typename U>
struct pointer_has_rebind_other
{
  template <typename V>
  struct any
  { any(const V&) {}};

  template <typename X>
  static char test(int, typename X::template rebind<U>::other*);

  template <typename X>
  static int test(any<int>, void);

  static const bool value = (1 == sizeof(test<Ptr>(0, 0)));
};

template <typename Ptr, typename U>
struct pointer_rebind_mode
{
  static const unsigned int rebind = (unsigned int)pointer_has_rebind<Ptr, U>::value;
  static const unsigned int rebind_other = (unsigned int)pointer_has_rebind_other<Ptr, U>::value;
  static const unsigned int mode = rebind + rebind * rebind_other;
};

//struct pointer_rebinder
template <typename Ptr, typename U, unsigned int RebindMode>
struct pointer_rebinder;

template <typename Ptr, typename U>
struct pointer_rebinder<Ptr, U, 2u>
{
  typedef typename Ptr::template rebind<U>::other type;
};

template <typename Ptr, typename U>
struct pointer_rebinder<Ptr, U, 1u>
{
  typedef typename Ptr::template rebind<U>::type type;
};

// Specialization of pointer_rebinder if Ptr does not
// have its own rebind template but has a the form Ptr<A, An...>,
// where An... comprises zero or more type parameters.
// Many types fit this form, hence many pointers will get a
// reasonable default for rebind.
template <template<class, class...> class Ptr,
    typename A, class... An, class U>
struct pointer_rebinder<Ptr<A, An...>, U, 0u>
{
  typedef Ptr<U, An...> type;
};

template <template<class > class Ptr, typename A, class U>
struct pointer_rebinder<Ptr<A>, U, 0u>
{
  typedef Ptr<U> type;
};

template <typename Ptr, typename U>
struct pointer_rebind :
    public pointer_rebinder<Ptr, U, pointer_rebind_mode<Ptr, U>::mode>
{};

template <typename T, typename U>
struct pointer_rebind<T*, U>
{ typedef U* type; };

}
}
}

#endif //AUTODDS_LIBS_INTRUSIVE_POINTER_REBIND_HPP_