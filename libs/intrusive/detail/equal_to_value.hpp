//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<typename ConstReference>
class euqal_to_value
{
  ConstReference t_;

 public:
  euqal_to_value(ConstReference t)
      : t_(t)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE bool operator ()(ConstReference t) const
  { return t_ == t; }
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_EQUAL_TO_VALUE_HPP_