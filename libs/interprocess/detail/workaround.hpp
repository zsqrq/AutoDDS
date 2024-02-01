//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_WORKAROUND_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_WORKAROUND_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config//config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#if defined(AUTODDS_INTERPROCESS_FORCE_NATIVE_EMULATION) && defined(AUTODDS_INTERPROCESS_FORCE_GENERIC_EMULATION)
#error "AUTODDS_INTERPROCESS_FORCE_NATIVE_EMULATION && AUTODDS_INTERPROCESS_FORCE_GENERIC_EMULATION can't be defined at the same time"
#endif


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define AUTODDS_INTERPROCESS_WINDOWS
#else
#include <unistd.h>

//////////////////////////////////////////////////////
//Check for XSI shared memory objects. They are available in nearly all UNIX platforms
//////////////////////////////////////////////////////
#if !defined(__QNXNTO__) && !defined(__ANDROID__) && !defined(__HAIKU__) && !(__VXWORKS__)
#define AUTODDS_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS
#endif

//////////////////////////////////////////////////////
// From SUSv3/UNIX 98, pthread_mutexattr_settype is mandatory
//////////////////////////////////////////////////////
#if defined(_XOPEN_UNIX) && ((_XOPEN_VERSION + 0) >= 500)
#define AUTODDS_INTERPROCESS_POSIX_RECURSIVE_MUTEXES
#endif

//////////////////////////////////////////////////////
// _POSIX_THREAD_PROCESS_SHARED (POSIX.1b/POSIX.4)
//////////////////////////////////////////////////////
#if defined(_POSIX_THREAD_PROCESS_SHARED) && ((_POSIX_THREAD_PROCESS_SHARED + 0) > 0)
//Cygwin defines _POSIX_THREAD_PROCESS_SHARED but does not implement it.
#if defined(__CYGWIN__)
#define AUTODDS_INTERPROCESS_BUGGY_POSIX_PROCESS_SHARED
#elif defined(__APPLE__)
//The pthreads implementation of darwin stores a pointer to a mutex inside the condition
         //structure so real sharing between processes is broken. See:
         //https://opensource.apple.com/source/libpthread/libpthread-301.30.1/src/pthread_cond.c.auto.html
         //in method pthread_cond_wait
         #define AUTODDS_INTERPROCESS_BUGGY_POSIX_PROCESS_SHARED
#endif

//If buggy _POSIX_THREAD_PROCESS_SHARED is detected avoid using it
#if defined(AUTODDS_INTERPROCESS_BUGGY_POSIX_PROCESS_SHARED)
#undef AUTODDS_INTERPROCESS_BUGGY_POSIX_PROCESS_SHARED
#else
#define AUTODDS_INTERPROCESS_POSIX_PROCESS_SHARED
#endif
#endif

//////////////////////////////////////////////////////
//    AUTODDS_INTERPROCESS_POSIX_ROBUST_MUTEXES
//////////////////////////////////////////////////////
#if (_XOPEN_SOURCE >= 700 || _POSIX_C_SOURCE >= 200809L)
#define AUTODDS_INTERPROCESS_POSIX_ROBUST_MUTEXES
#endif

//////////////////////////////////////////////////////
// _POSIX_SHARED_MEMORY_OBJECTS (POSIX.1b/POSIX.4)
//////////////////////////////////////////////////////
#if ( defined(_POSIX_SHARED_MEMORY_OBJECTS) && ((_POSIX_SHARED_MEMORY_OBJECTS + 0) > 0) ) ||\
         (defined(__vms) && __CRTL_VER >= 70200000)
#define AUTODDS_INTERPROCESS_POSIX_SHARED_MEMORY_OBJECTS
//Some systems have filesystem-based resources, so the
//portable "/shmname" format does not work due to permission issues
//For those systems we need to form a path to a temporary directory:
//          hp-ux               tru64               vms               freebsd
#if defined(__hpux) || defined(__osf__) || defined(__vms) || (defined(__FreeBSD__) && (__FreeBSD__ < 7))
#define AUTODDS_INTERPROCESS_FILESYSTEM_BASED_POSIX_SHARED_MEMORY
      //Some systems have "jailed" environments where shm usage is restricted at runtime
      //and temporary file based shm is possible in those executions.
#elif defined(__FreeBSD__)
#define AUTODDS_INTERPROCESS_RUNTIME_FILESYSTEM_BASED_POSIX_SHARED_MEMORY
#endif
#endif

//////////////////////////////////////////////////////
// _POSIX_MAPPED_FILES (POSIX.1b/POSIX.4)
//////////////////////////////////////////////////////
#if defined(_POSIX_MAPPED_FILES) && ((_POSIX_MAPPED_FILES + 0) > 0)
#define AUTODDS_INTERPROCESS_POSIX_MAPPED_FILES
#endif

//////////////////////////////////////////////////////
// _POSIX_SEMAPHORES (POSIX.1b/POSIX.4)
//////////////////////////////////////////////////////
#if ( defined(_POSIX_SEMAPHORES) && ((_POSIX_SEMAPHORES + 0) > 0) ) ||\
       ( defined(__FreeBSD__) && (__FreeBSD__ >= 4)) || \
         defined(__APPLE__)
#define AUTODDS_INTERPROCESS_POSIX_NAMED_SEMAPHORES
//MacOsX declares _POSIX_SEMAPHORES but sem_init returns ENOSYS
#if !defined(__APPLE__)
#define AUTODDS_INTERPROCESS_POSIX_UNNAMED_SEMAPHORES
#endif
#if defined(__osf__) || defined(__vms)
#define AUTODDS_INTERPROCESS_FILESYSTEM_BASED_POSIX_SEMAPHORES
#endif
#endif

//////////////////////////////////////////////////////
// _POSIX_BARRIERS (SUSv3/Unix03)
//////////////////////////////////////////////////////
#if defined(_POSIX_BARRIERS) && ((_POSIX_BARRIERS + 0) >= 200112L)
#define AUTODDS_INTERPROCESS_POSIX_BARRIERS
#endif

//////////////////////////////////////////////////////
// _POSIX_TIMEOUTS (SUSv3/Unix03)
//////////////////////////////////////////////////////
#if defined(_POSIX_TIMEOUTS) && ((_POSIX_TIMEOUTS + 0L) >= 200112L)
#define AUTODDS_INTERPROCESS_POSIX_TIMEOUTS
#endif

//////////////////////////////////////////////////////
// Detect BSD derivatives to detect sysctl
//////////////////////////////////////////////////////
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__APPLE__)
#define AUTODDS_INTERPROCESS_BSD_DERIVATIVE
      //Some *BSD systems (OpenBSD & NetBSD) need sys/param.h before sys/sysctl.h, whereas
      //others (FreeBSD & Darwin) need sys/types.h
      #include <sys/types.h>
      #include <sys/param.h>
      #include <sys/sysctl.h>
      #if defined(CTL_KERN) && defined (KERN_BOOTTIME)
         //#define AUTODDS_INTERPROCESS_HAS_KERNEL_BOOTTIME
      #endif
#endif

//////////////////////////////////////////////////////
//64 bit offset
//////////////////////////////////////////////////////
#if (defined (_V6_ILP32_OFFBIG)  &&(_V6_ILP32_OFFBIG   - 0 > 0)) ||\
       (defined (_V6_LP64_OFF64)    &&(_V6_LP64_OFF64     - 0 > 0)) ||\
       (defined (_V6_LPBIG_OFFBIG)  &&(_V6_LPBIG_OFFBIG   - 0 > 0)) ||\
       (defined (_XBS5_ILP32_OFFBIG)&&(_XBS5_ILP32_OFFBIG - 0 > 0)) ||\
       (defined (_XBS5_LP64_OFF64)  &&(_XBS5_LP64_OFF64   - 0 > 0)) ||\
       (defined (_XBS5_LPBIG_OFFBIG)&&(_XBS5_LPBIG_OFFBIG - 0 > 0)) ||\
       (defined (_FILE_OFFSET_BITS) &&(_FILE_OFFSET_BITS  - 0 >= 64))||\
       (defined (_FILE_OFFSET_BITS) &&(_FILE_OFFSET_BITS  - 0 >= 64))
#define AUTODDS_INTERPROCESS_UNIX_64_BIT_OR_BIGGER_OFF_T
#endif

//////////////////////////////////////////////////////
//posix_fallocate
//////////////////////////////////////////////////////
#if (_XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L)
#define AUTODDS_INTERPROCESS_POSIX_FALLOCATE
#endif

#endif   //!defined(AUTODDS_INTERPROCESS_WINDOWS)

#if defined(AUTODDS_INTERPROCESS_WINDOWS) || defined(AUTODDS_INTERPROCESS_POSIX_MAPPED_FILES)
#  define AUTODDS_INTERPROCESS_MAPPED_FILES
#endif

#if defined(AUTODDS_INTERPROCESS_POSIX_NAMED_SEMAPHORES) && !defined(AUTODDS_INTERPROCESS_POSIX_SEMAPHORES_NO_UNLINK)
#define AUTODDS_INTERPROCESS_NAMED_MUTEX_USES_POSIX_SEMAPHORES
#define AUTODDS_INTERPROCESS_NAMED_SEMAPHORE_USES_POSIX_SEMAPHORES
#endif

#if    !defined(AUTODDS_NO_CXX11_RVALUE_REFERENCES) && !defined(AUTODDS_NO_CXX11_VARIADIC_TEMPLATES)
#define AUTODDS_INTERPROCESS_PERFECT_FORWARDING
#endif

#ifndef AUTODDS_INTERPROCESS_TIMEOUT_WHEN_LOCKING_DURATION_MS
#define AUTODDS_INTERPROCESS_TIMEOUT_WHEN_LOCKING_DURATION_MS 10000
#endif

#ifndef AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_MAX_TRIES
#define AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_MAX_TRIES 20u
#endif

#ifndef AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_TIMEOUT_SEC
#define AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_TIMEOUT_SEC 300u
#endif

#define AUTODDS_INTERPROCESS_MSG_QUEUE_CIRCULAR_INDEX
#define AUTODDS_INTERPROCESS_IMPDEF(TYPE) TYPE
#define AUTODDS_INTERPROCESS_SEEDOC(TYPE) TYPE
#define AUTODDS_INTERPROCESS_DOC1ST(TYPE1, TYPE2) TYPE2
#define AUTODDS_INTERPROCESS_I ,
#define AUTODDS_INTERPROCESS_DOCIGN(T1) T1

#if defined(AUTODDS_INTERPROCESS_DISABLE_FORCEINLINE)
#define AUTODDS_INTERPROCESS_FORCEINLINE inline
#elif defined(AUTODDS_INTERPROCESS_FORCEINLINE_IS_AUTODDS_FORCELINE)
#define AUTODDS_INTERPROCESS_FORCEINLINE AUTODDS_FORCEINLINE
#elif defined(AUTODDS_MSVC) && (_MSC_VER < 1900 || defined(_DEBUG))
//"__forceinline" and MSVC seems to have some bugs in old versions and in debug mode
   #define AUTODDS_INTERPROCESS_FORCEINLINE inline
#elif defined(AUTODDS_GCC) && (__GNUC__ <= 5)
//Older GCCs have problems with forceinline
   #define AUTODDS_INTERPROCESS_FORCEINLINE inline
#else
#define AUTODDS_INTERPROCESS_FORCEINLINE AUTODDS_FORCEINLINE
#endif

#if defined(AUTODDS_HAS_THREADS)
#  if defined(_MSC_VER) || defined(__MWERKS__) || defined(__MINGW32__) ||  defined(__BORLANDC__)
     //no reentrant posix functions (eg: localtime_r)
#  elif (!defined(__hpux) || (defined(__hpux) && defined(_REENTRANT)))
#   define AUTODDS_INTERPROCESS_HAS_REENTRANT_STD_FUNCTIONS
#  endif
#endif


#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_WORKAROUND_HPP_