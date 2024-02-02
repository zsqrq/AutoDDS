//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/detail/shared_dir_helpers.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_