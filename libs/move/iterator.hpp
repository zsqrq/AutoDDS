//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_ITERATOR_HPP_
#define AUTODDS_LIBS_MOVE_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"
#include "libs/move/detail/iterator_traits.hpp"
#include "libs/move/utility_core.hpp"

namespace autodds {
namespace libs {

//! Class template move_iterator is an iterator adaptor with the same behavior
//! as the underlying iterator except that its dereference operator implicitly
//! converts the value returned by the underlying iterator's dereference operator
//! to an rvalue reference. Some generic algorithms can be called with move
//! iterators to replace copying with moving.

} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_ITERATOR_HPP_