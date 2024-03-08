//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SYNC_LOCK_OPTIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SYNC_LOCK_OPTIONS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

//!Type to indicate to a mutex lock constructor that must not lock the mutex.
struct defer_lock_type {};

//!Type to indicate to a mutex lock constructor that must try to lock the mutex.
struct try_to_lock_type {};

//!Type to indicate to a mutex lock constructor that the mutex is already locked.
struct accept_ownership_type{};

static const defer_lock_type       defer_lock       = defer_lock_type();
static const try_to_lock_type      try_to_lock      = try_to_lock_type();
static const accept_ownership_type accept_ownership = accept_ownership_type();

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_SYNC_LOCK_OPTIONS_HPP_