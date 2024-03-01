//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_MEMORY_IMPL_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_MEMORY_IMPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/segment_manager.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_MEMORY_IMPL_HPP_