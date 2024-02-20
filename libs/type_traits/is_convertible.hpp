//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_CONVERTIBLE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_CONVERTIBLE_HPP_

#include "libs/type_traits/intrinsics.hpp"
#include "libs/type_traits/integral_constant.hpp"
#include "libs/type_traits/is_complete.hpp"
#include "libs/type_traits/is_void.hpp"
#include "libs/type_traits/is_array.hpp"
#include "libs/common/static_assert.hpp"

#include "libs/type_traits/detail/yes_no_type.hpp"
#include "libs/type_traits/detail/config.hpp"
#include "libs/type_traits/is_arithmetic.hpp"
#include "libs/type_traits/is_abstract.hpp"
#include "libs/type_traits/add_lvalue_reference.hpp"
#include "libs/type_traits/add_rvalue_reference.hpp"
#include "libs/type_traits/detail/is_function_cxx_11.hpp"
#include "libs/type_traits/declval.hpp"

namespace autodds {
namespace libs {

#ifndef AUTODDS_IS_CONVERTIBLE

namespace detail {

#define AUTODDS_TT_CXX11_IS_CONVERTIBLE
template<typename A, typename B, typename C>
struct or_helper
{
  static const bool value = (A::value || B::value || C::value);
};

template <typename From, typename To, bool b = or_helper<is_void<From>, is_function<To>, is_array<To>>::value>
struct is_convertible_basic_impl
{
  static const bool value = is_void<To>::value;
};

template <typename From, typename To>
class is_convertible_basic_impl<From, To, false>
{
  typedef char one;
  typedef int two;

  template<typename To1>
  static void test_aux(To1);

  template<typename From1, typename To1>
  static decltype(test_aux<To1>(::autodds::libs::declval<From1>()), one()) test(int);

  template<typename, typename >
  static two test(...);
 public:
  static const bool value = sizeof(test<From, To>(0)) == 1;
};

template <typename From, typename To>
struct is_convertible_impl
{
  AUTODDS_STATIC_CONSTANT(bool, value = ( is_convertible_basic_impl<From, To>::value && !is_array<To>::value && !is_function<To>::value));
};

template <bool trivial1, bool trivial2, bool abstract_target>
struct is_convertible_impl_select
{
  template <typename From, typename To>
  struct rebind
  {
    typedef is_convertible_impl<From, To> type;
  };
};

template <>
struct is_convertible_impl_select<true, true, false>
{
  template<typename From, typename To>
  struct rebind
  {
    typedef true_type type;
  };
};

template <>
struct is_convertible_impl_select<false, false, true>
{
  template <typename From, typename To>
  struct rebind
  {
    typedef false_type type;
  };
};

template <>
struct is_convertible_impl_select<true, false, true>
{
  template<typename From, typename To>
  struct rebind {
    typedef false_type type;
  };
};

template <typename From, typename To>
struct is_convertible_impl_dispatch_base
{
  typedef is_convertible_impl_select<is_arithmetic<From>::value,
                                     is_arithmetic<To>::value,
                                     false> selector;
  typedef typename selector::template rebind<From, To> isc_binder;
  typedef typename isc_binder::type type;
};

template <typename From, typename To>
struct is_convertible_impl_dispatch
    : public is_convertible_impl_dispatch_base<From, To>::type
{};

template <> struct is_convertible_impl_dispatch<void, void> : public true_type{};
template <> struct is_convertible_impl_dispatch<void, void const> : public true_type{};
template <> struct is_convertible_impl_dispatch<void, void const volatile> : public true_type{};
template <> struct is_convertible_impl_dispatch<void, void volatile> : public true_type{};

template <> struct is_convertible_impl_dispatch<void const, void> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const, void const> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const, void const volatile> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const, void volatile> : public true_type{};

template <> struct is_convertible_impl_dispatch<void const volatile, void> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const volatile, void const> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const volatile, void const volatile> : public true_type{};
template <> struct is_convertible_impl_dispatch<void const volatile, void volatile> : public true_type{};

template <> struct is_convertible_impl_dispatch<void volatile, void> : public true_type{};
template <> struct is_convertible_impl_dispatch<void volatile, void const> : public true_type{};
template <> struct is_convertible_impl_dispatch<void volatile, void const volatile> : public true_type{};
template <> struct is_convertible_impl_dispatch<void volatile, void volatile> : public true_type{};

template <class To> struct is_convertible_impl_dispatch<void, To> : public false_type{};
template <class From> struct is_convertible_impl_dispatch<From, void> : public false_type{};
template <class To> struct is_convertible_impl_dispatch<void const, To> : public false_type{};
template <class From> struct is_convertible_impl_dispatch<From, void const> : public false_type{};
template <class To> struct is_convertible_impl_dispatch<void const volatile, To> : public false_type{};
template <class From> struct is_convertible_impl_dispatch<From, void const volatile> : public false_type{};
template <class To> struct is_convertible_impl_dispatch<void volatile, To> : public false_type{};
template <class From> struct is_convertible_impl_dispatch<From, void volatile> : public false_type{};

} // namespace detail

template <class From, class To>
struct is_convertible : public integral_constant<bool, detail::is_convertible_impl_dispatch<From, To>::value>
{
  AUTODDS_STATIC_ASSERT_MSG(is_complete<To>::value || is_void<To>::value || is_array<To>::value, "Destination argument type to is_convertible must be a complete type");
  AUTODDS_STATIC_ASSERT_MSG(is_complete<From>::value ||is_void<From>::value || is_array<From>::value, "From argument type to is_convertible must be a complete type");
};

#endif

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_CONVERTIBLE_HPP_