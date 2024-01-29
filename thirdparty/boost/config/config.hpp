//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HPP_
// if we don't have a user config, then use the default location:
#if !defined(BOOST_USER_CONFIG) && !defined(BOOST_NO_USER_CONFIG)
#  define BOOST_USER_CONFIG "boost/config/config/user.hpp"
#if 0
// For dependency trackers:
#  include <boost/config/user.hpp>
#endif
#endif
// include it first:
#ifdef BOOST_USER_CONFIG
#  include BOOST_USER_CONFIG
#endif

// if we don't have a compiler config set, try and find one:
#if !defined(BOOST_COMPILER_CONFIG) && !defined(BOOST_NO_COMPILER_CONFIG) && !defined(BOOST_NO_CONFIG)
#  include "boost/config/config//detail/select_compiler_config.hpp"
#endif
// if we have a compiler config, include it now:
#ifdef BOOST_COMPILER_CONFIG
#  include BOOST_COMPILER_CONFIG
#endif

// if we don't have a std library config set, try and find one:
#if !defined(BOOST_STDLIB_CONFIG) && !defined(BOOST_NO_STDLIB_CONFIG) && !defined(BOOST_NO_CONFIG) && defined(__cplusplus)
#  include "boost/config/config/detail/select_stdlib_config.hpp"
#endif
// if we have a std library config, include it now:
#ifdef BOOST_STDLIB_CONFIG
#  include BOOST_STDLIB_CONFIG
#endif

// if we don't have a platform config set, try and find one:
#if !defined(BOOST_PLATFORM_CONFIG) && !defined(BOOST_NO_PLATFORM_CONFIG) && !defined(BOOST_NO_CONFIG)
#  include "boost/config/config/detail/select_platform_config.hpp"
#endif
// if we have a platform config, include it now:
#ifdef BOOST_PLATFORM_CONFIG
#  include BOOST_PLATFORM_CONFIG
#endif

// get config suffix code:
#include <boost/config/detail/suffix.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HPP_