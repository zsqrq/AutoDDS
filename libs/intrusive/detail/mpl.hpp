//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/type_traits.hpp"
#include "libs/move/detail/meta_utils.hpp"

namespace autodds {
namespace intrusive {
namespace detail {

using autodds::libs::move_detail::is_same;
using autodds::libs::move_detail::add_const;
using autodds::libs::move_detail::remove_const;
using autodds::libs::move_detail::remove_cv;
using autodds::libs::move_detail::remove_reference;
using autodds::libs::move_detail::add_reference;
using autodds::libs::move_detail::remove_pointer;
using autodds::libs::move_detail::add_pointer;
using autodds::libs::move_detail::true_type;
using autodds::libs::move_detail::false_type;
using autodds::libs::move_detail::voider;
using autodds::libs::move_detail::enable_if_c;
using autodds::libs::move_detail::enable_if;
using autodds::libs::move_detail::disable_if_c;
using autodds::libs::move_detail::disable_if;
using autodds::libs::move_detail::is_convertible;
using autodds::libs::move_detail::if_c;
using autodds::libs::move_detail::if_;
using autodds::libs::move_detail::is_const;
using autodds::libs::move_detail::identity;
using autodds::libs::move_detail::alignment_of;
using autodds::libs::move_detail::is_empty;
using autodds::libs::move_detail::addressof;
using autodds::libs::move_detail::integral_constant;
using autodds::libs::move_detail::enable_if_convertible;
using autodds::libs::move_detail::disable_if_convertible;
using autodds::libs::move_detail::bool_;
using autodds::libs::move_detail::true_;
using autodds::libs::move_detail::false_;
using autodds::libs::move_detail::yes_type;
using autodds::libs::move_detail::no_type;
using autodds::libs::move_detail::apply;
using autodds::libs::move_detail::eval_if_c;
using autodds::libs::move_detail::eval_if;
using autodds::libs::move_detail::unvoid_ref;
using autodds::libs::move_detail::add_const_if_c;

template <std::size_t N>
struct ls_zeros
{
  static const std::size_t value = (N & std::size_t(1)) ? 0 : (1 + ls_zeros<(N >> 1u)>::value);
};

template<>
struct ls_zeros<0>
{
  static const std::size_t value = 0;
};

template <>
struct ls_zeros<1>
{
  static const std::size_t value = 0;
};

// Infrastructure for providing a default type for T::TNAME if absent.
#define AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(TNAME)   \
   template <typename T>                                          \
   struct autodds_intrusive_has_type_ ## TNAME                    \
   {                                                              \
      template <typename X>                                       \
      static char test(int, typename X::TNAME*);                  \
                                                                  \
      template <typename X>                                       \
      static int test(...);                                       \
                                                                  \
      static const bool value = (1 == sizeof(test<T>(0, 0)));     \
   };                                                             \
                                                                  \
   template <typename T, typename DefaultType>                    \
   struct autodds_intrusive_default_type_ ## TNAME                \
   {                                                              \
      struct DefaultWrap { typedef DefaultType TNAME; };          \
                                                                  \
      typedef typename                                            \
         ::autodds::intrusive::detail::if_c                       \
            < autodds_intrusive_has_type_ ## TNAME<T>::value      \
            , T, DefaultWrap>::type::TNAME type;                  \
   };                                                             \
   //

#define AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT(INSTANTIATION_NS_PREFIX, T, TNAME, TIMPL)   \
      typename INSTANTIATION_NS_PREFIX                                                         \
         autodds_intrusive_default_type_ ## TNAME< T, TIMPL >::type                            \
//

#define AUTODDS_INTRUSIVE_HAS_TYPE(INSTANTIATION_NS_PREFIX, T, TNAME)    \
      INSTANTIATION_NS_PREFIX                                            \
         autodds_intrusive_has_type_ ## TNAME< T >::value              \
//

#define AUTODDS_INTRUSIVE_INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(TNAME)\
   template <typename T, typename DefaultType>                    \
   struct autodds_intrusive_eval_default_type_ ## TNAME             \
   {                                                              \
      template <typename X>                                       \
      static char test(int, typename X::TNAME*);                  \
                                                                  \
      template <typename X>                                       \
      static int test(...);                                       \
                                                                  \
      struct DefaultWrap                                          \
      { typedef typename DefaultType::type TNAME; };              \
                                                                  \
      static const bool value = (1 == sizeof(test<T>(0, 0)));     \
                                                                  \
      typedef typename                                            \
         ::autodds::intrusive::detail::eval_if_c                    \
            < value                                               \
            , ::autodds::intrusive::detail::identity<T>             \
            , ::autodds::intrusive::detail::identity<DefaultWrap>   \
            >::type::TNAME type;                                  \
   };                                                             \
//

#define AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_EVAL_DEFAULT(INSTANTIATION_NS_PREFIX, T, TNAME, TIMPL) \
      typename INSTANTIATION_NS_PREFIX                                                          \
         autodds_intrusive_eval_default_type_ ## TNAME< T, TIMPL >::type                          \
//

#define AUTODDS_INTRUSIVE_INTERNAL_STATIC_BOOL_IS_TRUE(TRAITS_PREFIX, TYPEDEF_TO_FIND) \
template <class T>                                                                     \
struct TRAITS_PREFIX##_bool                                                            \
{                                                                                      \
   template<bool Add>                                                                  \
   struct two_or_three {autodds::libs::move_detail::yes_type _[2u + (unsigned)Add];};  \
   template <class U> static autodds::libs::move_detail::yes_type test(...);           \
   template <class U> static two_or_three<U::TYPEDEF_TO_FIND> test (int);              \
   static const std::size_t value = sizeof(test<T>(0));                                \
};                                                                                     \
                                                                                       \
template <class T>                                                                     \
struct TRAITS_PREFIX##_bool_is_true                                                    \
{                                                                                      \
   static const bool value = TRAITS_PREFIX##_bool<T>::value >                          \
   sizeof(autodds::libs::move_detail::yes_type)*2;                                     \
};                                                                                     \
//

#define AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(TRAITS_NAME, FUNC_NAME) \
  template <typename U, typename Signature> \
  class TRAITS_NAME \
  { \
  private: \
  template<Signature> struct helper;\
  template<typename T> \
  static ::autodds::intrusive::detail::yes_type test(helper<&T::FUNC_NAME>*); \
  template<typename T> static ::autodds::intrusive::detail::no_type test(...); \
  public: \
  static const bool value = sizeof(test<U>(0)) == sizeof(::autodds::intrusive::detail::yes_type); \
  }; \
//

}
}
}

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_MPL_HPP_