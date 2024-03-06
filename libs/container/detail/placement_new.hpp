//
// Created by wz on 24-3-5.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_PLACEMENT_NEW_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_PLACEMENT_NEW_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
struct autodds_container_new_t{};

inline void* operator new(std::size_t, void* p, autodds_container_new_t)
{ return p; }

inline void operator delete(void*, void*, autodds_container_new_t)
{}

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_PLACEMENT_NEW_HPP_