//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_INTERPROCESS_TESTER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_INTERPROCESS_TESTER_HPP_

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
namespace ipcdetail {

class interprocess_tester
{
 public:
  template<class  T>
  static void dont_close_on_destruction(T& t)
  { t.dont_close_on_destruction(); }
};

} // namespace ipcdetail
} // namespace libs
} // namespace interprocess
} // namesoace ipcdetail

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_INTERPROCESS_TESTER_HPP_