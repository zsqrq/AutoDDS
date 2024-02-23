//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_THREAD_FUNCTIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_THREAD_FUNCTIONS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/streams/bufferstream.hpp"

#include <cstddef>
#include <ostream>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

#if defined(CLOCK_MONOTONIC_RAW)
#     define AUTODDS_INTERPROCESS_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_MONOTONIC)
#     define AUTODDS_INTERPROCESS_CLOCK_MONOTONIC CLOCK_MONOTONIC
#  else
#     error "No high resolution steady clock in your system, please provide a patch"
#  endif

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

typedef pthread_t OS_thread_t;
typedef pthread_t OS_thread_id_t;
typedef pid_t     OS_process_id_t;

struct OS_systemwide_thread_id_t
{
  OS_systemwide_thread_id_t()
      :  pid(), tid()
  {}

  OS_systemwide_thread_id_t(pid_t p, pthread_t t)
      :  pid(p), tid(t)
  {}

  OS_systemwide_thread_id_t(const OS_systemwide_thread_id_t& other)
      :  pid(other.pid), tid(other.tid)
  {}

  OS_systemwide_thread_id_t(const volatile OS_systemwide_thread_id_t& other)
      :  pid(other.pid), tid(other.tid)
  {}

  OS_systemwide_thread_id_t & operator = (const OS_systemwide_thread_id_t& other)
  {  pid = other.pid;   tid = other.tid;   return *this;   }

  OS_systemwide_thread_id_t & operator = (const volatile OS_systemwide_thread_id_t& other)
  {  pid = other.pid;   tid = other.tid;   return *this;  }

  void operator=(const OS_systemwide_thread_id_t& other) volatile
  {  pid = other.pid;   tid = other.tid;   }

  pid_t       pid;
  pthread_t   tid;
};

inline void systemwide_thread_id_copy(const volatile OS_systemwide_thread_id_t& source,
                                      volatile OS_systemwide_thread_id_t& dist)
{
  dist.pid = source.pid;
  dist.tid = source.tid;
}

inline OS_process_id_t get_current_process_id()
{ return ::getpid(); }

inline OS_process_id_t get_invalid_process_id()
{ return pid_t (0); }

inline OS_thread_id_t get_current_thread_id()
{ return ::pthread_self(); }

inline OS_thread_id_t get_invalid_thread_id()
{
  static pthread_t invalid_id;
  return invalid_id;
}

inline bool equal_thread_id(OS_thread_id_t id1, OS_thread_id_t id2)
{ return 0 != pthread_equal(id1, id2); }

inline void thread_yield()
{ ::sched_yield(); }

typedef struct timespec OS_highres_count_t;

inline unsigned long get_system_tick_ns()
{
#ifdef _SC_CLK_TCK
  long ticks_per_second = ::sysconf(_SC_CLK_TCK);
  if (ticks_per_second <= 0)
  {
    ticks_per_second = 100;
  }
  return 999999999ul/static_cast<unsigned long>(ticks_per_second)+1ul;
#else
#error "Can't obtain system tick value for your system, please provide a patch"
#endif
}

inline unsigned long get_system_tick_in_highres_counts()
{ return get_system_tick_ns(); }

inline unsigned long get_system_tick_us()
{ return (get_system_tick_ns() - 1) / 1000ul + 1ul; }

inline OS_highres_count_t get_current_system_highres_count()
{
#if defined(AUTODDS_INTERPROCESS_CLOCK_MONOTONIC)
  struct timespec count;
  ::clock_gettime(AUTODDS_INTERPROCESS_CLOCK_MONOTONIC, &count);
  return count;
#endif
}

inline void zero_highres_count(OS_highres_count_t& count)
{ count.tv_sec = 0; count.tv_nsec = 0; }

inline bool is_highres_count_zero(const OS_highres_count_t& count)
{ return count.tv_sec == 0 && count.tv_nsec == 0; }

template <typename Ostream>
inline Ostream &ostream_highres_count(Ostream& ostream, const OS_highres_count_t& count)
{
  ostream << count.tv_sec << "s:" << count.tv_nsec << "ns";
  return ostream;
}

inline OS_highres_count_t system_highres_count_subtract(const OS_highres_count_t& l, const OS_highres_count_t& r)
{
  OS_highres_count_t res;
  if (l.tv_nsec < r.tv_nsec)
  {
    res.tv_nsec = 1000000000 + l.tv_nsec - r.tv_nsec;
    res.tv_sec = l.tv_sec - 1 - r.tv_sec;
  }
  else
  {
    res.tv_nsec = l.tv_nsec - r.tv_nsec;
    res.tv_sec  = l.tv_sec - r.tv_sec;
  }
  return res;
}

inline bool system_highres_count_less(const OS_highres_count_t &l, const OS_highres_count_t &r)
{  return l.tv_sec < r.tv_sec || (l.tv_sec == r.tv_sec && l.tv_nsec < r.tv_nsec);  }

inline bool system_highres_count_less_ul(const OS_highres_count_t &l, unsigned long r)
{  return !l.tv_sec && (static_cast<unsigned long>(l.tv_nsec) < r);  }

inline void thread_sleep_tick()
{
  OS_highres_count_t tts;
  tts.tv_sec = 0;
  tts.tv_nsec = (long)get_system_tick_ns()/2;
  ::nanosleep(&tts, 0);
}

inline void thread_sleep(unsigned int ms)
{
  OS_highres_count_t  tts;
  tts.tv_sec = ms/1000u;
  tts.tv_nsec = (ms%1000u)*1000000u;
  ::nanosleep(&tts, 0);
}

// systemwide thread
inline OS_systemwide_thread_id_t get_current_systemwide_thread_id()
{
  return OS_systemwide_thread_id_t(::getpid(), ::pthread_self());
}

inline bool equal_systemwide_thread_id(const OS_systemwide_thread_id_t& id1,
                                       const OS_systemwide_thread_id_t& id2)
{
  return (0 != pthread_equal(id1.tid, id2.tid)) && (id1.pid == id2.pid);
}

inline OS_systemwide_thread_id_t get_invalid_systemwide_thread_id()
{
  return OS_systemwide_thread_id_t(get_invalid_process_id(), get_invalid_thread_id());
}

inline long double get_current_process_creation_time()
{ return 0.0L; }

inline unsigned get_num_cores()
{
#ifdef _SC_NPROCESSORS_ONLN

  long cores = ::sysconf(_SC_NPROCESSORS_ONLN);
  // sysconf returns -1 if the name is invalid, the option does not exist or
  // does not have a definite limit.
  // if sysconf returns some other negative number, we have no idea
  // what is going on. Default to something safe.
  if (cores <= 0)
  {
    return 1;
  } else if (static_cast<unsigned long>(cores) >=
  static_cast<unsigned long >(static_cast<unsigned int >(-1)))
  {
    return static_cast<unsigned int>(-1);
  }
  else
  {
    return static_cast<unsigned int>(cores);
  }

#endif
}

inline int thread_create(OS_thread_t* thread, void* (*start_coutine) (void*), void* arg)
{ return pthread_create(thread, 0, start_coutine, arg); }

inline void thread_join(OS_thread_t thread)
{ (void) pthread_join(thread, 0); }

typedef char pid_str_t[sizeof(OS_process_id_t)*3 + 1];

inline void get_pid_str(pid_str_t &pid_str, OS_process_id_t pid)
{
  bufferstream bstream(pid_str, sizeof(pid_str));
  bstream << pid << std::ends;
}

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_THREAD_FUNCTIONS_HPP_