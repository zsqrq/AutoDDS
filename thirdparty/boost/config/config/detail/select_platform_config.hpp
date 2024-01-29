//
// Created by wz on 24-1-29.
//

#ifndef AUTODDS_THIRDPARTY_BOOST_CONFIG_DETAIL_SELECT_PLATFORM_CONFIG_HPP_
#define AUTODDS_THIRDPARTY_BOOST_CONFIG_DETAIL_SELECT_PLATFORM_CONFIG_HPP_
//  Boost compiler configuration selection header file

//  (C) Copyright John Maddock 2001 - 2002.
//  (C) Copyright Jens Maurer 2001.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

// locate which platform we are on and define BOOST_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not
// <header_name> in order to prevent macro expansion within the header
// name (for example "linux" is a macro on linux systems).

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/linux.hpp"

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/bsd.hpp"

#elif defined(sun) || defined(__sun)
// solaris:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/solaris.hpp"

#elif defined(__sgi)
// SGI Irix:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/irix.hpp"

#elif defined(__hpux)
// hp unix:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/hpux.hpp"

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/cygwin.hpp"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/win32.hpp"

#elif defined(__HAIKU__)
// Haiku
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/haiku.hpp"

#elif defined(__BEOS__)
// BeOS
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/beos.hpp"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/macos.hpp"

#elif defined(__TOS_MVS__)
// IBM z/OS
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/zos.hpp"

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM AIX
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/aix.hpp"

#elif defined(__amigaos__)
// AmigaOS
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/amigaos.hpp"

#elif defined(__QNXNTO__)
// QNX:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/qnxnto.hpp"

#elif defined(__VXWORKS__)
// vxWorks:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/vxworks.hpp"

#elif defined(__SYMBIAN32__)
// Symbian:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/symbian.hpp"

#elif defined(_CRAYC)
// Cray:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/cray.hpp"

#elif defined(__VMS)
// VMS:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/vms.hpp"

#elif defined(__CloudABI__)
// Nuxi CloudABI:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/cloudabi.hpp"

#elif defined (__wasm__)
// Web assembly:
#  define BOOST_PLATFORM_CONFIG "boost/config/config/platform/wasm.hpp"

#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef BOOST_HAS_UNISTD_H
#     define BOOST_HAS_UNISTD_H
#  endif

#  include "boost/config/config/detail/posix_features.hpp"

#  endif

#  if defined (BOOST_ASSERT_CONFIG)
      // this must come last - generate an error if we don't
      // recognise the platform:
#     error "Unknown platform - please configure and report the results to boost.org"
#  endif

#endif

#if 0
//
// This section allows dependency scanners to find all the files we *might* include:
//
#  include "boost/config/config/platform/linux.hpp"
#  include "boost/config/config/platform/bsd.hpp"
#  include "boost/config/config/platform/solaris.hpp"
#  include "boost/config/config/platform/irix.hpp"
#  include "boost/config/config/platform/hpux.hpp"
#  include "boost/config/config/platform/cygwin.hpp"
#  include "boost/config/config/platform/win32.hpp"
#  include "boost/config/config/platform/beos.hpp"
#  include "boost/config/config/platform/macos.hpp"
#  include "boost/config/config/platform/zos.hpp"
#  include "boost/config/config/platform/aix.hpp"
#  include "boost/config/config/platform/amigaos.hpp"
#  include "boost/config/config/platform/qnxnto.hpp"
#  include "boost/config/config/platform/vxworks.hpp"
#  include "boost/config/config/platform/symbian.hpp"
#  include "boost/config/config/platform/cray.hpp"
#  include "boost/config/config/platform/vms.hpp"
#  include <boost/config/config/detail/posix_features.hpp>


#endif


#endif //AUTODDS_THIRDPARTY_BOOST_CONFIG_DETAIL_SELECT_PLATFORM_CONFIG_HPP_