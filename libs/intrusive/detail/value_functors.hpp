//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_VALUE_FUNCTORS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_VALUE_FUNCTORS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>

namespace autodds {
namespace libs {
namespace intrusive {

template<typename ValueType>
struct value_less
{
  bool operator()(const ValueType& a, const ValueType& b) const
  {
    return a < b;
  }
};

template<typename T>
struct value_less<T*>
{
  bool operator()(const T *a, const T* b) const
  {
    return std::size_t(a) < std::size_t(b);
  }
};

template<typename ValueType>
struct value_equal
{
  bool operator()(const ValueType &a, const ValueType &b) const
  {  return a == b;  }
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_VALUE_FUNCTORS_HPP_