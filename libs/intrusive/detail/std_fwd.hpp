//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_STD_FWD_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_STD_FWD_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/std_ns_begin.hpp"

AUTODDS_MOVE_STD_NS_BEG

template<class T>
struct less;

template<class T>
struct equal_to;

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

AUTODDS_MOVE_STD_NS_END

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_STD_FWD_HPP_