//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_ALLOCATION_TYPE_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_ALLOCATION_TYPE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace container {

enum allocation_type_v
{
  // constants for allocation commands
  allocate_new_v   = 0x01,
  expand_fwd_v     = 0x02,
  expand_bwd_v     = 0x04,
//   expand_both    = expand_fwd | expand_bwd,
//   expand_or_new  = allocate_new | expand_both,
  shrink_in_place_v = 0x08,
  nothrow_allocation_v = 0x10,
  zero_memory_v = 0x20,
  try_shrink_in_place_v = 0x40
};

typedef unsigned int allocation_type;

static const allocation_type allocate_new       = (allocation_type)allocate_new_v;
static const allocation_type expand_fwd         = (allocation_type)expand_fwd_v;
static const allocation_type expand_bwd         = (allocation_type)expand_bwd_v;
static const allocation_type shrink_in_place    = (allocation_type)shrink_in_place_v;
static const allocation_type try_shrink_in_place= (allocation_type)try_shrink_in_place_v;
static const allocation_type nothrow_allocation = (allocation_type)nothrow_allocation_v;
static const allocation_type zero_memory        = (allocation_type)zero_memory_v;

} // namespace container
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_ALLOCATION_TYPE_HPP_