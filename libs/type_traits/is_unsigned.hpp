//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_UNSIGNED_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_UNSIGNED_HPP_

#include "libs/type_traits/is_integral.hpp"
#include "libs/type_traits/is_enum.hpp"
#include "libs/type_traits/remove_cv.hpp"
#include <climits>

namespace autodds {
namespace libs {

namespace detail {

template <typename T>
struct is_unsigned_values
{
  typedef typename remove_cv<T>::type no_cv_t;
  static const no_cv_t minus_one = (static_cast<no_cv_t>(-1));
  static const no_cv_t zero = (static_cast<no_cv_t>(0));
};

template <typename T>
struct is_unsigned_helper
{
  AUTODDS_STATIC_CONSTANT(bool, value = (is_unsigned_values<T>::minus_one > is_unsigned_values<T>::zero));
};

template <bool integral_type>
struct is_unsigned_select_helper
{
  template<class T>
  struct rebind
  {
    typedef is_unsigned_helper<T> type;
  };
};

template <>
struct is_unsigned_select_helper<false>
{
  template<class T>
  struct rebind
  {
    typedef false_type type;
  };
};

template <typename T>
struct is_unsigned
{
  typedef is_unsigned_select_helper< ::autodds::libs::is_integral<T>::value
                                         || ::autodds::libs::is_enum<T>::value > selector;
  typedef typename selector::template rebind<T> binder;
  typedef typename binder::type type;
  AUTODDS_STATIC_CONSTANT(bool , value = type::value);
};

} // namespace detail

template <typename T>
struct is_unsigned : public integral_constant<bool, detail::is_unsigned<T>::value> {};

template <> struct is_unsigned<unsigned char> : public true_type{};
template <> struct is_unsigned<const unsigned char> : public true_type{};
template <> struct is_unsigned<volatile unsigned char> : public true_type{};
template <> struct is_unsigned<const volatile unsigned char> : public true_type{};
template <> struct is_unsigned<unsigned short> : public true_type{};
template <> struct is_unsigned<const unsigned short> : public true_type{};
template <> struct is_unsigned<volatile unsigned short> : public true_type{};
template <> struct is_unsigned<const volatile unsigned short> : public true_type{};
template <> struct is_unsigned<unsigned int> : public true_type{};
template <> struct is_unsigned<const unsigned int> : public true_type{};
template <> struct is_unsigned<volatile unsigned int> : public true_type{};
template <> struct is_unsigned<const volatile unsigned int> : public true_type{};
template <> struct is_unsigned<unsigned long> : public true_type{};
template <> struct is_unsigned<const unsigned long> : public true_type{};
template <> struct is_unsigned<volatile unsigned long> : public true_type{};
template <> struct is_unsigned<const volatile unsigned long> : public true_type{};

template <> struct is_unsigned<signed char> : public false_type{};
template <> struct is_unsigned<const signed char> : public false_type{};
template <> struct is_unsigned<volatile signed char> : public false_type{};
template <> struct is_unsigned<const volatile signed char> : public false_type{};
template <> struct is_unsigned< short> : public false_type{};
template <> struct is_unsigned<const  short> : public false_type{};
template <> struct is_unsigned<volatile  short> : public false_type{};
template <> struct is_unsigned<const volatile  short> : public false_type{};
template <> struct is_unsigned< int> : public false_type{};
template <> struct is_unsigned<const  int> : public false_type{};
template <> struct is_unsigned<volatile  int> : public false_type{};
template <> struct is_unsigned<const volatile  int> : public false_type{};
template <> struct is_unsigned< long> : public false_type{};
template <> struct is_unsigned<const  long> : public false_type{};
template <> struct is_unsigned<volatile  long> : public false_type{};
template <> struct is_unsigned<const volatile  long> : public false_type{};

#ifdef AUTODDS_HAS_LONG_LONG
template <> struct is_unsigned< ::autodds::ulong_long_type> : public true_type{};
template <> struct is_unsigned<const ::autodds::ulong_long_type> : public true_type{};
template <> struct is_unsigned<volatile ::autodds::ulong_long_type> : public true_type{};
template <> struct is_unsigned<const volatile ::autodds::ulong_long_type> : public true_type{};

template <> struct is_unsigned< ::autodds::long_long_type> : public false_type{};
template <> struct is_unsigned<const ::autodds::long_long_type> : public false_type{};
template <> struct is_unsigned<volatile ::autodds::long_long_type> : public false_type{};
template <> struct is_unsigned<const volatile ::autodds::long_long_type> : public false_type{};
#endif

#if defined(CHAR_MIN)
#if CHAR_MIN == 0
template <> struct is_unsigned<char> : public true_type{};
template <> struct is_unsigned<const char> : public true_type{};
template <> struct is_unsigned<volatile char> : public true_type{};
template <> struct is_unsigned<const volatile char> : public true_type{};
#else
template <> struct is_unsigned<char> : public false_type{};
template <> struct is_unsigned<const char> : public false_type{};
template <> struct is_unsigned<volatile char> : public false_type{};
template <> struct is_unsigned<const volatile char> : public false_type{};
#endif
#endif

#if !defined(AUTODDS_NO_INTRINSIC_WCHAR_T) && defined(WCHAR_MIN)
#if WCHAR_MIN == 0
template <> struct is_unsigned<wchar_t> : public true_type{};
template <> struct is_unsigned<const wchar_t> : public true_type{};
template <> struct is_unsigned<volatile wchar_t> : public true_type{};
template <> struct is_unsigned<const volatile wchar_t> : public true_type{};
#else
template <> struct is_unsigned<wchar_t> : public false_type{};
template <> struct is_unsigned<const wchar_t> : public false_type{};
template <> struct is_unsigned<volatile wchar_t> : public false_type{};
template <> struct is_unsigned<const volatile wchar_t> : public false_type{};
#endif
#endif

}
}
#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_UNSIGNED_HPP_