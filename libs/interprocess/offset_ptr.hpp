//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_
#define AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/detail/cast_tags.hpp"
#include "libs/interprocess/detail/mpl.hpp"

#include "libs/type_traits/is_convertible.hpp"
#include "libs/type_traits/is_constructible.hpp"
#include "libs/type_traits/is_integral.hpp"
#include "libs/type_traits/is_unsigned.hpp"

#include "libs/container/detail/type_traits.hpp"
#include "libs/common/assert.hpp"
#include "libs/common/static_assert.hpp"
#include <iosfwd>


#endif //AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_