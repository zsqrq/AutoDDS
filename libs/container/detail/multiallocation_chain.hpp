//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/container/container_fwd.hpp"
#include "libs/move/detail/to_raw_pointer.hpp"
#include "libs/container/detail/transform_iterator.hpp"
#include "libs/container/detail/type_traits.hpp"
#include "libs/container/detail/placement_new.hpp"
#include "libs/intrusive/slist.hpp"


#endif //AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_