//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP_


#include "libs/config/config.hpp"
#include "libs/config/workaround.hpp"

namespace mpl_ {

template <bool B> struct bool_;
template <class I, I val> struct integral_c;
struct integral_c_tag;

}

namespace autodds {
namespace mpl {

using ::mpl_::bool_;
using ::mpl_::integral_c;
using ::mpl_::integral_c_tag;

}
}

namespace autodds {
namespace libs {

template <typename T, T val>
struct integral_constant
{
  typedef mpl::integral_c_tag          tag;
  typedef T                            value_type;
  typedef integral_constant<T, val>    type;
  static const T value = val;

  operator const mpl::integral_c<T,val>& () const
  {
    static const char data[sizeof(long )] = {0};
    static const void* pdata = data;
    return *(reinterpret_cast<const mpl::integral_c<T,val>*>(pdata));
  }
  AUTODDS_CONSTEXPR operator T() const { return val; }
};

template <typename T, T val>
T const integral_constant<T, val>::value;

template <bool val>
struct integral_constant<bool, val>
{
  typedef mpl::integral_c_tag tag;
  typedef bool value_type;
  typedef integral_constant<bool, val> type;
  static const bool value = val;

  operator const mpl::bool_<val>& ()const
  {
    static const char data[sizeof(long)] = { 0 };
    static const void* pdata = data;
    return *(reinterpret_cast<const mpl::bool_<val>*>(pdata));
  }
  AUTODDS_CONSTEXPR operator bool()const { return val; }
};

template <bool val>
bool const integral_constant<bool, val>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP_