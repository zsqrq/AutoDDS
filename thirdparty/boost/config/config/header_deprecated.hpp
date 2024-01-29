//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HEADER_DEPRECATED_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HEADER_DEPRECATED_HPP_
//  Copyright 2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  BOOST_HEADER_DEPRECATED("<alternative>")
//
//  Expands to the equivalent of
//    BOOST_PRAGMA_MESSAGE("This header is deprecated. Use <alternative> instead.")
//
//  Note that this header is C compatible.

#include "boost/config/config/pragma_message.hpp"

#if defined(BOOST_ALLOW_DEPRECATED_HEADERS) || defined(BOOST_ALLOW_DEPRECATED)
# define BOOST_HEADER_DEPRECATED(a)
#else
# define BOOST_HEADER_DEPRECATED(a) BOOST_PRAGMA_MESSAGE("This header is deprecated. Use " a " instead.")
#endif

#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_HEADER_DEPRECATED_HPP_