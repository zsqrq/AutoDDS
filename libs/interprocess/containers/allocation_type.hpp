//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP_
#define AUTODDS_LIBS_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/container/detail/allocation_type.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

using autodds::libs::container::allocation_type;

static const allocation_type allocate_new       = autodds::libs::container::allocate_new;
static const allocation_type expand_fwd         = autodds::libs::container::expand_fwd;
static const allocation_type expand_bwd         = autodds::libs::container::expand_bwd;
static const allocation_type shrink_in_place    = autodds::libs::container::shrink_in_place;
static const allocation_type try_shrink_in_place= autodds::libs::container::try_shrink_in_place;
static const allocation_type nothrow_allocation = autodds::libs::container::nothrow_allocation;
static const allocation_type zero_memory        = autodds::libs::container::zero_memory;

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_CONTAINERS_ALLOCATION_TYPE_HPP_