//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_ASSERT_CXX98_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_ASSERT_CXX98_HPP_
//  This file was automatically generated on Wed Mar  3 08:46:11 2021
//  by libs/config/tools/generate.cpp
//  Copyright John Maddock 2002-4.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.//
//  Revision $Id$
//

#include "boost/config/config.hpp"
#include "boost/config/config/assert_cxx17.hpp"

#ifdef BOOST_NO_CXX98_BINDERS
#  error "Your compiler appears not to be fully C++98 compliant.  Detected via defect macro BOOST_NO_CXX98_BINDERS."
#endif
#ifdef BOOST_NO_CXX98_FUNCTION_BASE
#  error "Your compiler appears not to be fully C++98 compliant.  Detected via defect macro BOOST_NO_CXX98_FUNCTION_BASE."
#endif
#ifdef BOOST_NO_CXX98_RANDOM_SHUFFLE
#  error "Your compiler appears not to be fully C++98 compliant.  Detected via defect macro BOOST_NO_CXX98_RANDOM_SHUFFLE."
#endif


#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_ASSERT_CXX98_HPP_