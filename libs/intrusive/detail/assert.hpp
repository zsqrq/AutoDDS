//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_ASSERT_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_ASSERT_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if !defined(AUTODDS_INTRUSIVE_INVARIANT_ASSERT)
#include "libs/common/assert.hpp"
#define AUTODDS_INTRUSIVE_INVARIANT_ASSERT AUTODDS_ASSERT
#elif defined(AUTODDS_INTRUSIVE_INVARIANT_ASSERT_INCLUDE)
#include AUTODDS_INTRUSIVE_INVARIANT_ASSERT_INCLUDE
#endif

#if !defined(AUTODDS_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT)
#include "libs/common/assert.hpp"
#define AUTODDS_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT AUTODDS_ASSERT
#elif defined(AUTODDS_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT_INCLUDE)
#include AUTODDS_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT_INCLUDE
#endif

#if !defined(AUTODDS_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT)
#include "libs/common/assert.hpp"
#define AUTODDS_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT AUTODDS_ASSERT
#elif defined(AUTODDS_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT_INCLUDE)
#include AUTODDS_INTRUSIVE_SAFE_HOOK_DESTRUCTOR_ASSERT_INCLUDE
#endif

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_ASSERT_HPP_