//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP_
#define AUTODDS_LIBS_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/container/detail/version_type.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

using autodds::libs::container::dtl::version_type;
using autodds::libs::container::dtl::version;

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_CONTAINERS_VERSION_TYPE_HPP_