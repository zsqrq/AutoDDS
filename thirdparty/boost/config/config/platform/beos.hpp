//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_PLATFORM_BEOS_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_PLATFORM_BEOS_HPP_
//  (C) Copyright John Maddock 2001.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  BeOS specific config options:

#define BOOST_PLATFORM "BeOS"

#define BOOST_NO_CWCHAR
#define BOOST_NO_CWCTYPE
#define BOOST_HAS_UNISTD_H

#define BOOST_HAS_BETHREADS

#ifndef BOOST_DISABLE_THREADS
#  define BOOST_HAS_THREADS
#endif

// boilerplate code:
#include "boost/config/config/detail/posix_features.hpp"





#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_PLATFORM_BEOS_HPP_