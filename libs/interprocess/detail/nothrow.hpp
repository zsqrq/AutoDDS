//
// Created by wz on 24-3-4.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_NOTHROW_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_NOTHROW_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace std {
struct nothrow_t;
} // namespace std

namespace autodds {
namespace libs {
namespace interprocess {

template <int Dummy = 0>
struct nothrow
{
  static const std::nothrow_t& get()
  { return *pnothrow; }

  static std::nothrow_t* pnothrow;
};

template <int Dummy>
std::nothrow_t* nothrow<Dummy>::pnothrow =
    reinterpret_cast<std::nothrow_t*>(0x1234);

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_NOTHROW_HPP_