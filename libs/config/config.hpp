//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_CONFIG_HPP
#define AUTODDS_CONFIG_HPP

// if we don't have a user config, then use the default location:
#if !defined(AUTODDS_USER_CONFIG) && !defined(AUTODDS_NO_USER_CONFIG)
#  define AUTODDS_USER_CONFIG "libs/config/userconfig.hpp"
#if 0
// For dependency trackers:
#  include "libs/config/userconfig.hpp"
#endif
#endif

#ifdef AUTODDS_USER_CONFIG
#  include AUTODDS_USER_CONFIG
#endif

// if we don't have a compiler config set, try and find one:
#if !defined(AUTODDS_COMPILER_CONFIG) && !defined(AUTODDS_NO_COMPILER_CONFIG) && !defined(AUTODDS_NO_CONFIG)
#include "libs/config/detail/compiler_selection_config.hpp"
#endif

#ifdef AUTODDS_COMPILER_CONFIG
#  include AUTODDS_COMPILER_CONFIG
#endif
// if we don't have a std library config set, try and find one:
#if !defined(AUTODDS_STDLIB_CONFIG) && !defined(AUTODDS_NO_STDLIB_CONFIG) && !defined(AUTODDS_NO_CONFIG) && defined(__cplusplus)
#  include "libs/config/detail/stdlib_selection_config.hpp"
#endif

#ifdef AUTODDS_STDLIB_CONFIG
#include AUTODDS_STDLIB_CONFIG
#endif

// if we don't have a platform config set, try and find one:
#if !defined(AUTODDS_PLATFORM_CONFIG) && !defined(AUTODDS_NO_PLATFORM_CONFIG) && !defined(AUTODDS_NO_CONFIG)
#  include "libs/config/detail/platform_selection_config.hpp"
#endif

#ifdef AUTODDS_PLATFORM_CONFIG
#  include AUTODDS_PLATFORM_CONFIG
#endif

// get config suffix code:
#include "libs/config/detail/suffix.hpp"

#ifdef AUTODDS_HAS_PRAGMA_ONCE
#pragma once
#endif

#ifndef AUTODDS_ASSERT
#define AUTODDS_ASSERT assert
#endif

#ifndef AUTODDS_STATIC_ASSERT
#define AUTODDS_STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif

#endif //AUTODDS_LIBS_CONFIG_CONFIG_HPP_