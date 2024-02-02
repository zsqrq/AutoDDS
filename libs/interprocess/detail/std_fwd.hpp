//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_STD_FWD_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_STD_FWD_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/std_ns_begin.hpp"
AUTODDS_MOVE_STD_NS_BEG

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

template<class T>
struct char_traits;
template<class CharT, class Traits>
class basic_ostream;

template<class CharT, class Traits>
class basic_istream;

AUTODDS_MOVE_STD_NS_END
#include "libs/move/detail/std_ns_end.hpp"
#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_STD_FWD_HPP_