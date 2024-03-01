//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_CONFIG_BEGIN_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_CONFIG_BEGIN_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#


#if defined(AUTODDS_GCC) && (AUTODDS_GCC >= 40600)
#pragma GCC diagnostic push
//Sign conversion warnings broken before GCC 9.3
//(https://gcc.gnu.org/bugzilla/show_bug.cgi?id=87519)
#if AUTODDS_GCC < 90300
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
#endif

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_CONFIG_BEGIN_HPP_