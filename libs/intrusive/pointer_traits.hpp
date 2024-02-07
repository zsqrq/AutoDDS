//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_rebind.hpp"
#include "libs/move/detail/pointer_element.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include <cstddef>

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace intrusive {
namespace detail {

AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_pointer_to, pointer_to)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_dynamic_cast_from, dynamic_cast_from)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_static_cast_from, static_cast_from)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_const_cast_from, const_cast_from)

AUTODDS_INTRUSIVE_INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(element_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(difference_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(size_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(reference)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(value_traits_ptr)

}

//! pointer_traits is the implementation of C++11 std::pointer_traits class with some
//! extensions like castings.
//!
//! pointer_traits supplies a uniform interface to certain attributes of pointer-like types.
//!
//! <b>Note</b>: When defining a custom family of pointers or references to be used with BI
//! library, make sure the public static conversion functions accessed through
//! the `pointer_traits` interface (`*_cast_from` and `pointer_to`) can
//! properly convert between const and nonconst referred member types
//! <b>without the use of implicit constructor calls</b>. It is suggested these
//! conversions be implemented as function templates, where the template
//! argument is the type of the object being converted from.

template <typename Ptr>
struct pointer_traits
{
  typedef autodds::intrusive::detail::true_ true_;

  typedef autodds::intrusive::detail::false_ false_;

  typedef Ptr pointer;

  typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_EVAL_DEFAULT(
      autodds::intrusive::detail::, Ptr, element_type,
      autodds::movelib::detail::first_param<Ptr>)  element_type;

  typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT(
      autodds::intrusive::detail::, Ptr, difference_type, std::ptrdiff_t)   difference_type;

  typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT
  ( autodds::intrusive::detail::, Ptr, size_type
  , typename autodds::libs::move_detail::
        make_unsigned<difference_type>::type)                          size_type;

  typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT(
      autodds::intrusive::detail::, Ptr, reference,
      typename autodds::intrusive::detail::unvoid_ref<element_type>::type) reference;

  template<class U> struct rebind_pointer
  {
    typedef typename autodds::libs::intrusive::pointer_rebind<Ptr, U>::type type;
  };

  template<class U>
  using rebind = typename autodds::libs::intrusive::pointer_rebind<Ptr, U>::type;

  //!   If element_type is (possibly cv-qualified) void, r type is unspecified; otherwise,
  //!   it is element_type &.
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer pointer_to(reference ref) AUTODDS_NOEXCEPT
  {
    const bool value = autodds::intrusive::detail::has_member_function_callable_with_pointer_to
        <pointer, pointer(*)(reference)>::value;
    autodds::intrusive::detail::bool_<value> flag;
    return pointer_traits::priv_pointer_to(flag, ref);
  }

  //!   A dereferenceable pointer to r obtained by calling the static template function
  //!   Ptr::static_cast_from(UPpr/const UPpr &).
  //!   If such function does not exist, returns pointer_to(static_cast<element_type&>(*uptr))
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer static_cast_from(AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  {
    typedef const Uptr& RefArgs;
    const bool value = autodds::intrusive::detail::has_member_function_callable_with_static_cast_from
        <pointer, pointer(*)(RefArgs)>::value || autodds::intrusive::detail::
        has_member_function_callable_with_static_cast_from<pointer, pointer(*)(Uptr)>::value;
    return pointer_traits::priv_static_cast_from(autodds::intrusive::detail::bool_<value>(), uptr);
  }

  //!   A dereferenceable pointer to r obtained by calling the static template function
  //!   Ptr::const_cast_from<UPtr>(UPpr/const UPpr &).
  //!   If such function does not exist, returns pointer_to(const_cast<element_type&>(*uptr))
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer const_cast_from(AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  {
    typedef const Uptr& RefArgs;
    const bool value = autodds::intrusive::detail::has_member_function_callable_with_const_cast_from
        <pointer, pointer(*)(RefArgs)>::value || autodds::intrusive::detail::
        has_member_function_callable_with_const_cast_from<pointer, pointer(*)(Uptr)>::value;
    return pointer_traits::priv_const_cast_from(autodds::intrusive::detail::bool_<value>(), uptr);
  }

  //!   A dereferenceable pointer to r obtained by calling the static template function
  //!   Ptr::dynamic_cast_from<UPtr>(UPpr/const UPpr &).
  //!   If such function does not exist, returns pointer_to(*dynamic_cast<element_type*>(&*uptr))
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer dynamic_cast_from(AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  {
    typedef const Uptr& RefArgs;
    const bool value = autodds::intrusive::detail::has_member_function_callable_with_dynamic_cast_from
        <pointer, pointer(*)(RefArgs)>::value || autodds::intrusive::detail::
        has_member_function_callable_with_dynamic_cast_from<pointer, pointer(*)(RefArgs)>::value;
    return pointer_traits::priv_dynamic_cast_from(autodds::intrusive::detail::bool_<value>(), uptr);
  }

 private:
  // to raw pointer
  template<class T>
  AUTODDS_INTRUSIVE_FORCEINLINE static T* to_raw_pointer(T* p) AUTODDS_NOEXCEPT
  { return p; }

  template<class Pointer>
  AUTODDS_INTRUSIVE_FORCEINLINE static typename pointer_traits<Pointer>::element_type*
  to_raw_pointer(AUTODDS_LV_CONST_REF(Pointer, p)) AUTODDS_NOEXCEPT
  { return pointer_traits::to_raw_pointer(p.operator->()); }

  // priv pointer to
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_pointer_to(true_, reference ref) AUTODDS_NOEXCEPT
  { return Ptr::pointer_to(ref); }

  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_pointer_to(false_, reference ref) AUTODDS_NOEXCEPT
  { return pointer(autodds::intrusive::detail::addressof(ref)); }

  // priv static cast from
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_static_cast_from(true_, AUTODDS_LV_CONST_REF(Uptr,uptr)) AUTODDS_NOEXCEPT
  { return Ptr::static_cast_from(uptr); }

  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_static_cast_from(false_, AUTODDS_LV_CONST_REF(Uptr,uptr)) AUTODDS_NOEXCEPT
  { return uptr ? pointer_to(*static_cast<element_type*>(to_raw_pointer(uptr))) : pointer(); }

  // priv const cast from
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_const_cast_from(true_, AUTODDS_LV_CONST_REF(Uptr,uptr)) AUTODDS_NOEXCEPT
  { return Ptr::const_cast_from(uptr); }

  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_const_cast_from(false_, AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  { return uptr ? pointer_to(const_cast<element_type&>(*uptr)) : pointer(); }

  // priv dynamic cast from
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_dynamic_cast_from(true_, AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  { return Ptr::dynamic_cast_from(uptr); }

  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer priv_dynamic_cast_from(false_, AUTODDS_LV_CONST_REF(Uptr, uptr)) AUTODDS_NOEXCEPT
  { return uptr ? pointer_to(dynamic_cast<element_type&>(*uptr)) : pointer(); }
};

// Remove cv qualification from Ptr parameter to pointer_traits:
template <typename Ptr>
struct pointer_traits<const Ptr> : pointer_traits<Ptr> {};
template <typename Ptr>
struct pointer_traits<volatile Ptr> : pointer_traits<Ptr> {};
template <typename Ptr>
struct pointer_traits<const volatile Ptr> : pointer_traits<Ptr> {};
template <typename Ptr>
struct pointer_traits<Ptr&> : pointer_traits<Ptr> {};

//! Specialization of pointer_traits for raw pointers
template <typename T>
struct pointer_traits<T*>
{
  typedef T                  element_type;
  typedef T*                 pointer;
  typedef std::ptrdiff_t     difference_type;
  typedef std::size_t        size_type;

  typedef typename autodds::intrusive::detail::unvoid_ref<element_type>::type reference;
  template<class U> using rebind = U*;

  template<class U> struct rebind_pointer
  { typedef U* type; };

  // addressof
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer pointer_to(reference ref) AUTODDS_NOEXCEPT
  { return autodds::intrusive::detail::addressof(ref); }

  //! static_cast<pointer>(uptr)
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer static_cast_from(Uptr* uptr) AUTODDS_NOEXCEPT
  { return static_cast<pointer>(uptr); }

  //! const_cast<pointer>(uptr)
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer const_cast_from(Uptr* uptr) AUTODDS_NOEXCEPT
  { return const_cast<pointer>(uptr); }

  //! dynamic_cast<pointer>(uptr)
  template<class Uptr>
  AUTODDS_INTRUSIVE_FORCEINLINE static pointer dynamic_cast_from(Uptr* uptr) AUTODDS_NOEXCEPT
  { return dynamic_cast<pointer>(uptr); }

};

}
}

#endif //AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_