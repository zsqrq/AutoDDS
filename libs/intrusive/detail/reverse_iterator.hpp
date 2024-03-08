//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_REVERSE_ITERATOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_REVERSE_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/reverse_iterator.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

using autodds::libs::movelib::reverse_iterator;
using autodds::libs::movelib::make_reverse_iterator;

} // namespace intrusive
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_REVERSE_ITERATOR_HPP_