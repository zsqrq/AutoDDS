//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CORE_NO_EXCEPTIONS_SUPPORT_HPP_
#define AUTODDS_LIBS_CORE_NO_EXCEPTIONS_SUPPORT_HPP_
#include "libs/config/config.hpp"
#include "libs/config/workaround.hpp"


#if !(defined AUTODDS_NO_EXCEPTIONS)
#    define AUTODDS_TRY { try
#    define AUTODDS_CATCH(x) catch(x)
#    define AUTODDS_RETHROW throw;
#    define AUTODDS_CATCH_END }
#endif

#endif //AUTODDS_LIBS_CORE_NO_EXCEPTIONS_SUPPORT_HPP_