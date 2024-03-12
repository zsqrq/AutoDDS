//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_
#define AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/core/addressof.hpp"
#include "libs/common/assert.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/sync/scoped_lock.hpp"
#include "libs/interprocess/containers/allocation_type.hpp"
#include "libs/container/detail/multiallocation_chain.hpp"
#include "libs/interprocess/mem_algo/detail/mem_algo_common.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_