//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_SIMPLE_SWAP_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_SIMPLE_SWAP_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace interprocess {

template<typename T>
void simple_swap(T& x, T& y)
{
  T tmp(x);
  x = y;
}

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_SIMPLE_SWAP_HPP_