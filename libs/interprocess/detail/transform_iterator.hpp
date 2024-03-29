//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_TRANSFORM_ITERATOR_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_TRANSFORM_ITERATOR_HPP_

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
#include "libs/container/detail/transform_iterator.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

using autodds::libs::container::make_transform_iterator;
using autodds::libs::container::transform_iterator;

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_TRANSFORM_ITERATOR_HPP_