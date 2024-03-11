//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_TWIN_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_TWIN_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {

template<typename T>
struct twin
{
  typedef T type;
  twin() : first(), second()
  {}

  twin(const type& f, const type& s)
  : first(f), second(s)
  {}

  T first;
  T second;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_TWIN_HPP_