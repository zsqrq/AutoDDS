//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_INTEGRAL_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_INTEGRAL_HPP_

#include "libs/config/config.hpp"
#include "libs/type_traits/integral_constant.hpp"

namespace autodds {
namespace libs {

template <typename T> 
struct is_integral : public false_type {};
template <typename T>
struct is_integral<const T> : public is_integral<T>{};
template <typename T>
struct is_integral<volatile const T> : public is_integral<T>{};
template <typename T>
struct is_integral<volatile T> : public is_integral<T> {};

template <>
struct is_integral<unsigned char> : public true_type {};
template <> 
struct is_integral<unsigned short > : public true_type {};
template <>
struct is_integral<unsigned int> : public true_type {};
template <>
struct is_integral<unsigned long> : public true_type {};

template <>
struct is_integral<signed char> : public true_type {};
template <>
struct is_integral<short> : public true_type {};
template <>
struct is_integral<int> : public true_type {};
template <>
struct is_integral<long> : public true_type {};

template <>
struct is_integral<char> : public true_type {};
template <>
struct is_integral<bool> : public true_type {};
template<> 
struct is_integral<wchar_t> : public true_type{};

# if defined(AUTODDS_HAS_LONG_LONG)
template<> struct is_integral< ::autodds::ulong_long_type> : public true_type{};
template<> struct is_integral< ::autodds::long_long_type> : public true_type{};
#elif defined(AUTODDS_HAS_MS_INT64)
template<> struct is_integral<unsigned __int64> : public true_type{};
template<> struct is_integral<__int64> : public true_type{};
#endif

#ifdef AUTODDS_HAS_INT128
template<> struct is_integral<autodds::int128_type> : public true_type{};
template<> struct is_integral<autodds::uint128_type> : public true_type{};
#endif

#ifndef AUTODDS_NO_CXX11_CHAR16_T
template<> struct is_integral<char16_t> : public true_type{};
#endif
#ifndef AUTODDS_NO_CXX11_CHAR32_T
template<> struct is_integral<char32_t> : public true_type{};
#endif
#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
template<> struct is_integral<char8_t> : public true_type{};
#endif

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_INTEGRAL_HPP_