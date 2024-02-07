//
// Created by wz on 24-2-7.
//

#ifndef AUTODDS_LIBS_COMMON_ASSERT_HPP_
#define AUTODDS_LIBS_COMMON_ASSERT_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#undef AUTODDS_ASSERT
#undef AUTODDS_ASSERT_MSG
#undef AUTODDS_ASSERT_IS_VOID

#if defined(AUTODDS_DISABLE_ASSERTS) || ( defined(AUTODDS_ENABLE_ASSERT_DEBUG_HANDLER) && defined(NDEBUG) )

# define AUTODDS_ASSERT(expr) ((void)0)
# define AUTODDS_ASSERT_MSG(expr, msg) ((void)0)
# define AUTODDS_ASSERT_IS_VOID

#elif defined(AUTODDS_ENABLE_ASSERT_HANDLER) || ( defined(AUTODDS_ENABLE_ASSERT_DEBUG_HANDLER) && !defined(NDEBUG) )

#include <boost/config.hpp> // for AUTODDS_LIKELY
#include <boost/current_function.hpp>

namespace boost
{
    void assertion_failed(char const * expr, char const * function, char const * file, long line); // user defined
    void assertion_failed_msg(char const * expr, char const * msg, char const * function, char const * file, long line); // user defined
} // namespace boost

#define AUTODDS_ASSERT(expr) (AUTODDS_LIKELY(!!(expr))? ((void)0): ::boost::assertion_failed(#expr, AUTODDS_CURRENT_FUNCTION, __FILE__, __LINE__))
#define AUTODDS_ASSERT_MSG(expr, msg) (AUTODDS_LIKELY(!!(expr))? ((void)0): ::boost::assertion_failed_msg(#expr, msg, AUTODDS_CURRENT_FUNCTION, __FILE__, __LINE__))

#else

# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same

# define AUTODDS_ASSERT(expr) assert(expr)
# define AUTODDS_ASSERT_MSG(expr, msg) assert((expr)&&(msg))
#if defined(NDEBUG)
# define AUTODDS_ASSERT_IS_VOID
#endif

#endif

#undef AUTODDS_VERIFY
#undef AUTODDS_VERIFY_MSG

#if defined(AUTODDS_DISABLE_ASSERTS) || ( !defined(AUTODDS_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define AUTODDS_VERIFY(expr) ((void)(expr))
# define AUTODDS_VERIFY_MSG(expr, msg) ((void)(expr))

#else

# define AUTODDS_VERIFY(expr) AUTODDS_ASSERT(expr)
# define AUTODDS_VERIFY_MSG(expr, msg) AUTODDS_ASSERT_MSG(expr,msg)

#endif

#endif //AUTODDS_LIBS_COMMON_ASSERT_HPP_