//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/min_max.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/common/static_assert.hpp"
#include "libs/common/cstdint.hpp"
#include <climits>

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <typename T>
inline T* to_raw_pointer(T* p)
{ return p;}



}
}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_