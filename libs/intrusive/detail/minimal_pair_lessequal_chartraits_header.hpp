//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_MINIMAL_PAIR_LESSEQUAL_CHARTRAITS_HEADER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_MINIMAL_PAIR_LESSEQUAL_CHARTRAITS_HEADER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if defined(AUTODDS_GNU_STDLIB)
#include <bits/stl_pair.h>
#include <bits/stl_function.h>
#include <bits/char_traits.h>
#else
#  include <utility>    //Fallback
#endif

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_MINIMAL_PAIR_LESSEQUAL_CHARTRAITS_HEADER_HPP_