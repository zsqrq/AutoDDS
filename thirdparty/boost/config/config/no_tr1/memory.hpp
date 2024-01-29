//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_NO_TR1_MEMORY_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_NO_TR1_MEMORY_HPP_
//  (C) Copyright John Maddock 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// The aim of this header is just to include <memory> but to do
// so in a way that does not result in recursive inclusion of
// the Boost TR1 components if boost/tr1/tr1/memory is in the
// include search path.  We have to do this to avoid circular
// dependencies:
//

#ifndef BOOST_CONFIG_MEMORY
#  define BOOST_CONFIG_MEMORY

#  ifndef BOOST_TR1_NO_RECURSION
#     define BOOST_TR1_NO_RECURSION
#     define BOOST_CONFIG_NO_MEMORY_RECURSION
#  endif

#  include <memory>

#  ifdef BOOST_CONFIG_NO_MEMORY_RECURSION
#     undef BOOST_TR1_NO_RECURSION
#     undef BOOST_CONFIG_NO_MEMORY_RECURSION
#  endif

#endif


#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_CONFIG_NO_TR1_MEMORY_HPP_