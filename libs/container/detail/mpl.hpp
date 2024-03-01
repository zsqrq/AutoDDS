//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_MPL_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_MPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/move/detail/type_traits.hpp"
#include "libs/intrusive/detail/mpl.hpp"

#include <cstddef>

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

using autodds::libs::move_detail::integral_constant;
using autodds::libs::move_detail::true_type;
using autodds::libs::move_detail::false_type;
using autodds::libs::move_detail::enable_if_c;
using autodds::libs::move_detail::enable_if;
using autodds::libs::move_detail::enable_if_convertible;
using autodds::libs::move_detail::disable_if_c;
using autodds::libs::move_detail::disable_if;
using autodds::libs::move_detail::disable_if_convertible;
using autodds::libs::move_detail::is_convertible;
using autodds::libs::move_detail::if_c;
using autodds::libs::move_detail::if_;
using autodds::libs::move_detail::identity;
using autodds::libs::move_detail::bool_;
using autodds::libs::move_detail::true_;
using autodds::libs::move_detail::false_;
using autodds::libs::move_detail::yes_type;
using autodds::libs::move_detail::no_type;
using autodds::libs::move_detail::bool_;
using autodds::libs::move_detail::true_;
using autodds::libs::move_detail::false_;
using autodds::libs::move_detail::unvoid_ref;
using autodds::libs::move_detail::and_;
using autodds::libs::move_detail::or_;
using autodds::libs::move_detail::not_;
using autodds::libs::move_detail::enable_if_and;
using autodds::libs::move_detail::disable_if_and;
using autodds::libs::move_detail::enable_if_or;
using autodds::libs::move_detail::disable_if_or;
using autodds::libs::move_detail::remove_const;

template <typename FirstType>
struct select1st
{
  typedef FirstType type;

  template <typename T>
  AUTODDS_CONTAINER_FORCEINLINE const type& operator()(const T& x) const
  {  return x.first;   }

  template<class T>
  AUTODDS_CONTAINER_FORCEINLINE type& operator()(T& x)
  {  return const_cast<type&>(x.first);   }
};

template <typename T>
struct void_t { typedef void type; };

template <typename T, typename = void>
struct is_transparent_base
{
  static const bool value = false;
};

template <typename T>
struct is_transparent_base<T, typename void_t<typename T::is_transparent>::type>
{
  static const bool value = true;
};

template <typename T>
struct is_transparent
    : is_transparent_base<T>
{};

template <typename C, typename /*Dummy*/, typename R>
struct enable_if_transparent
    : autodds::libs::move_detail::enable_if_c<is_transparent<C>::value, R>
{};

#ifndef AUTODDS_CONTAINER_NO_CXX17_CTAD

// void_t (void_t for C++11)
template<typename...> using variadic_void_t = void;

// Trait to detect Allocator-like types.
template <typename Allocator, typename = void>
struct is_allocator
{
  static const bool value = false;
};

template <typename T>
T&& ctad_declval();

template <typename Allocator>
struct is_allocator < Allocator,
                      variadic_void_t< typename Allocator::value_type
                          , decltype(ctad_declval<Allocator&>().allocate(size_t{})) >>
{
  static const bool value = true;
};

template<class T>
using require_allocator_t = typename enable_if_c<is_allocator<T>::value, T>::type;

template<class T>
using require_nonallocator_t = typename enable_if_c<!is_allocator<T>::value, T>::type;

#endif

} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds

#include "libs/container/detail/config_end.hpp"

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_MPL_HPP_