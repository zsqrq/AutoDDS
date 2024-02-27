//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SYNC_SPIN_WAIT_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SYNC_SPIN_WAIT_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/os_thread_functions.hpp"

#ifdef AUTODDS_INTERPROCESS_SPIN_WAIT_DEBUG
#include <iostream>
#endif

#ifdef defined(__GNUC__) && ( defined(__i386__) || defined(__x86_64__) ) && !defined(_CRAYC)

#define AUTODDS_INTERPROCESS_SMT_PAUSE __asm__ __volatile__( "rep; nop" : : : "memory" );

#endif

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <int Dummy = 0>
class num_core_holder
{
 public:
  static unsigned int get()
  {
    if (!num_cores)
    {
      return get_num_cores();
    } else
    {
      return num_cores;
    }
  }
 private:
  static unsigned int num_cores;
};

template <int Dummy>
unsigned int num_core_holder<Dummy>::num_cores = ipcdetail::get_num_cores();

} // namespace ipcdetail

class spin_wait
{
 public:
  static const unsigned int nop_pause_limit = 32u;

  spin_wait()
      : m_count_start(), m_ul_yield_only_counts(), m_k()
  {}

#ifdef AUTODDS_INTERPROCESS_SPIN_WAIT_DEBUG
  ~spin_wait()
  {
    if (m_k)
      {
        std::cout << "final m_k: " << m_k
                   << " system tick(us): " << ipcdetail::get_system_tick_us() << std::endl;
      }
  }
#endif

  unsigned int count() const
  { return m_k; }

  void yield()
  {
    // lazy initialization
    if (!m_k)
    {
      this->init_limits();
    }
    if (m_k < (nop_pause_limit >> 2)){

    }
#if defined(AUTODDS_INTERPROCESS_SMT_PAUSE)
    else if (m_k < nop_pause_limit) {
      AUTODDS_INTERPROCESS_SMT_PAUSE
    }
#endif
    // Yield strategy
    else
    {
      if (m_k < nop_pause_limit) {
        this->init_tick_info();
      } else if (this->yield_or_sleep()) {
        ipcdetail::thread_yield();
      } else {
        ipcdetail::thread_sleep_tick();
      }
    }
    ++m_k;
  }

  void reset()
  { m_k = 0; }

 private:

  void init_limits()
  {
    unsigned int num_cores = ipcdetail::num_core_holder<0>::get();
    m_k = num_cores > 1u ? 0u : nop_pause_limit;
  }

  void init_tick_info()
  {
    m_ul_yield_only_counts = ipcdetail::get_system_tick_in_highres_counts();
    m_count_start = ipcdetail::get_current_system_highres_count();
  }

  //Returns true if yield must be called, false is sleep must be called
  bool yield_or_sleep()
  {
    if (!m_ul_yield_only_counts) {
      return (m_k & 1u) != 0;
    }
    else
    {
      const ipcdetail::OS_highres_count_t now = ipcdetail::get_current_system_highres_count();
      const ipcdetail::OS_highres_count_t elapsed = ipcdetail::system_highres_count_subtract(now, m_count_start);

      if (!ipcdetail::system_highres_count_less_ul(elapsed, m_ul_yield_only_counts))
      {
#ifdef AUTODDS_INTERPROCESS_SPIN_WAIT_DEBUG
        std::cout << "elapsed!\n"
                      << "  m_ul_yield_only_counts: " << m_ul_yield_only_counts
                     << " system tick(us): " << ipcdetail::get_system_tick_us() << '\n'
                      << "  m_k: " << m_k << " elapsed counts: ";
                     ipcdetail::ostream_highres_count(std::cout, elapsed) << std::endl;
#endif
        m_ul_yield_only_counts = 0ul;
        return false;
      }
    }
    return true;
  }

  ipcdetail::OS_highres_count_t m_count_start;
  unsigned long m_ul_yield_only_counts;
  unsigned int m_k;
};

} // namespace libs
} // namespace interprocess
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_SYNC_SPIN_WAIT_HPP_