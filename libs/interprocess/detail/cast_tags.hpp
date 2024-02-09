//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_CAST_TAGS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_CAST_TAGS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};
struct reinterpret_cast_tag {};

}
}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_CAST_TAGS_HPP_