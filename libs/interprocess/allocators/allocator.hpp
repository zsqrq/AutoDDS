//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_
#define AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_

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
#include "libs/interprocess/containers/allocation_type.hpp"
#include "libs/container/detail/multiallocation_chain.hpp"
#include "libs/interprocess/allocators/detail/allocator_common.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/containers/version_type.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/common/assert.hpp"
#include "libs/core/addressof.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/container/detail/placement_new.hpp"

#include <cstddef>
#include <stdexcept>

#endif //AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_