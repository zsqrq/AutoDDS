//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_DETAIL_POSIX_FEATURES_HPP_
#define AUTODDS_LIBS_CONFIG_DETAIL_POSIX_FEATURES_HPP_

#  if defined(AUTODDS_HAS_UNISTD_H)
#     include <unistd.h>

      // XOpen has <nl_types.h>, but is this the correct version check?
#     if defined(_XOPEN_VERSION) && (_XOPEN_VERSION >= 3)
#        define AUTODDS_HAS_NL_TYPES_H
#     endif

      // POSIX version 6 requires <stdint.h>
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200100)
#        define AUTODDS_HAS_STDINT_H
#     endif

      // POSIX version 2 requires <dirent.h>
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199009L)
#        define AUTODDS_HAS_DIRENT_H
#     endif

      // POSIX version 3 requires <signal.h> to have sigaction:
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199506L)
#        define AUTODDS_HAS_SIGACTION
#     endif
      // POSIX defines _POSIX_THREADS > 0 for pthread support,
      // however some platforms define _POSIX_THREADS without
      // a value, hence the (_POSIX_THREADS+0 >= 0) check.
      // Strictly speaking this may catch platforms with a
      // non-functioning stub <pthreads.h>, but such occurrences should
      // occur very rarely if at all.
#     if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(AUTODDS_HAS_WINTHREADS) && !defined(AUTODDS_HAS_MPTASKS)
#        define AUTODDS_HAS_PTHREADS
#     endif

      // AUTODDS_HAS_NANOSLEEP:
      // This is predicated on _POSIX_TIMERS or _XOPEN_REALTIME:
#     if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS+0 >= 0)) \
             || (defined(_XOPEN_REALTIME) && (_XOPEN_REALTIME+0 >= 0))
#        define AUTODDS_HAS_NANOSLEEP
#     endif

      // AUTODDS_HAS_CLOCK_GETTIME:
      // This is predicated on _POSIX_TIMERS (also on _XOPEN_REALTIME
      // but at least one platform - linux - defines that flag without
      // defining clock_gettime):
#     if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS+0 >= 0))
#        define AUTODDS_HAS_CLOCK_GETTIME
#     endif

      // AUTODDS_HAS_SCHED_YIELD:
      // This is predicated on _POSIX_PRIORITY_SCHEDULING or
      // on _POSIX_THREAD_PRIORITY_SCHEDULING or on _XOPEN_REALTIME.
#     if defined(_POSIX_PRIORITY_SCHEDULING) && (_POSIX_PRIORITY_SCHEDULING+0 > 0)\
            || (defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING+0 > 0))\
            || (defined(_XOPEN_REALTIME) && (_XOPEN_REALTIME+0 >= 0))
#        define AUTODDS_HAS_SCHED_YIELD
#     endif

      // AUTODDS_HAS_GETTIMEOFDAY:
      // AUTODDS_HAS_PTHREAD_MUTEXATTR_SETTYPE:
      // These are predicated on _XOPEN_VERSION, and appears to be first released
      // in issue 4, version 2 (_XOPEN_VERSION > 500).
      // Likewise for the functions log1p and expm1.
#     if defined(_XOPEN_VERSION) && (_XOPEN_VERSION+0 >= 500)
#        define AUTODDS_HAS_GETTIMEOFDAY
#        if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE+0 >= 500)
#           define AUTODDS_HAS_PTHREAD_MUTEXATTR_SETTYPE
#        endif
#        ifndef AUTODDS_HAS_LOG1P
#           define AUTODDS_HAS_LOG1P
#        endif
#        ifndef AUTODDS_HAS_EXPM1
#           define AUTODDS_HAS_EXPM1
#        endif
#     endif

#  endif



#endif //AUTODDS_LIBS_CONFIG_DETAIL_POSIX_FEATURES_HPP_