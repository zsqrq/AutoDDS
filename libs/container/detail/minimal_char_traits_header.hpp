//
// Created by wz on 24-3-4.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if defined(_MSC_VER) && defined(AUTODDS_DINKUMWARE_STDLIB)
#include <iosfwd>   //Dinkum libraries for MSVC define std::char_traits there
#elif defined(AUTODDS_GNU_STDLIB)
#include <bits/char_traits.h>
#else
#include <string>  //Fallback
#endif

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_MINIMAL_CHAR_TRAITS_HEADER_HPP_