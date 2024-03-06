//
// Created by wz on 24-3-5.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/variadic_templates_tools.hpp"

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

using autodds::libs::container::dtl::tuple;
using autodds::libs::container::dtl::build_number_seq;
using autodds::libs::container::dtl::index_tuple;
using autodds::libs::container::dtl::get;

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_VARIADIC_TEMPLATES_TOOLS_HPP_