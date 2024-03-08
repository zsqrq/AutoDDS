//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_STD_FWD_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_STD_FWD_HPP_

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
class allocator;

template<class T>
struct less;

template<class T>
struct equal_to;

template<class T1, class T2>
struct pair;

template<class T>
struct char_traits;

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

template<class Container>
class insert_iterator;

struct allocator_arg_t;

struct piecewise_construct_t;

template <class Ptr>
struct pointer_traits;

AUTODDS_MOVE_STD_NS_END
#include "libs/move/detail/std_ns_end.hpp"

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_STD_FWD_HPP_