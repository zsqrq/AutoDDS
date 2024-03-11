//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_PLACEMENT_NEW_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_PLACEMENT_NEW_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

struct boost_move_new_t{};

//avoid including <new>
inline void *operator new(std::size_t, void *p, boost_move_new_t)
{  return p;  }

inline void operator delete(void *, void *, boost_move_new_t)
{}

#endif //AUTODDS_LIBS_MOVE_DETAIL_PLACEMENT_NEW_HPP_