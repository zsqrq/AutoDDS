//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/core/no_exceptions_support.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/transform_iterator.hpp"

#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/detail/nothrow.hpp"
#include "libs/interprocess/detail/segment_manager_helper.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_