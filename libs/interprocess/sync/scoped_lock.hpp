//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SYNC_SCOPED_LOCK_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SYNC_SCOPED_LOCK_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/sync/lock_options.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/utilities.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

template<class Mutex>
class scoped_lock
{
 private:
  typedef scoped_lock<Mutex>    this_type;
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(scoped_lock)
  typedef bool this_type::*unspecified_bool_type;

 public:
  typedef Mutex  mutex_type;

  scoped_lock() AUTODDS_NOEXCEPT
      : mp_mutex(0), m_locked(false)
  {}

  explicit scoped_lock(mutex_type& m)
      : mp_mutex(&m), m_locked(false)
  {
    mp_mutex->lock();
    m_locked = true;
  }

  scoped_lock(mutex_type& m, defer_lock_type)
      : mp_mutex(&m), m_locked(false)
  {}

  scoped_lock(mutex_type& m, accept_ownership_type)
      : mp_mutex(&m), m_locked(true)
  {}

  scoped_lock(mutex_type& m, try_to_lock_type)
      : mp_mutex(&m), m_locked(mp_mutex->try_lock())
  {}

  template<class TimePoint>
  scoped_lock(mutex_type& m, const TimePoint& abs_time)
      : mp_mutex(&m), m_locked(mp_mutex->timed_lock(abs_time))
  {}

  scoped_lock(AUTODDS_RV_REF(scoped_lock) scop) AUTODDS_NOEXCEPT
      : mp_mutex(0), m_locked(scop.owns())
  {  mp_mutex = scop.release(); }

  template<class T>
  explicit scoped_lock(AUTODDS_RV_REF(upgradable_lock<T>) upglock,
                       typename ipcdetail::enable_if<ipcdetail::is_same<T,Mutex>>::type* = 0)
      : mp_mutex(0), m_locked(false)
  {
    upgradable_lock<mutex_type>& u_lock = upglock;
    if (u_lock.owns())
    {
      u_lock.mutex()->unlock_upgradable_and_lock();
      m_locked = true;
    }
    mp_mutex = u_lock.lock();
  }

  template<class T, class TimePoint>
  scoped_lock(AUTODDS_RV_REF(upgradable_lock<T>) upglock, const TimePoint& abs_time,
              typename ipcdetail::enable_if<ipcdetail::is_same<T, Mutex>>::type* = 0)
      : mp_mutex(0), m_locked(false)
  {
    upgradable_lock<mutex_type> &u_lock = upglock;
    if(u_lock.owns()){
      if((m_locked = u_lock.mutex()->timed_unlock_upgradable_and_lock(abs_time)) == true){
        mp_mutex = u_lock.release();
      }
    }
    else{
      u_lock.release();
    }
  }

  template<class T>
  scoped_lock(AUTODDS_RV_REF(sharable_lock<T>) shar, try_to_lock_type
      , typename ipcdetail::enable_if< ipcdetail::is_same<T, Mutex> >::type * = 0)
      : mp_mutex(0), m_locked(false)
  {
    sharable_lock<mutex_type> &s_lock = shar;
    if(s_lock.owns()){
      if((m_locked = s_lock.mutex()->try_unlock_sharable_and_lock()) == true){
        mp_mutex = s_lock.release();
      }
    }
    else{
      s_lock.release();
    }
  }

  ~scoped_lock()
  {
    AUTODDS_TRY {
        if (m_locked && mp_mutex)
        {
          mp_mutex->unlock();
        }
      }
      AUTODDS_CATCH(...) {}
    AUTODDS_CATCH_END
  }

  scoped_lock& operator =( AUTODDS_RV_REF(scoped_lock) scop)
  {
    if (this->owns())
    {
      this->unlock();
    }
    m_locked = scop.owns();
    mp_mutex = scop.release();
    return *this;
  }

  void lock()
  {
    if (!mp_mutex || m_locked)
    {
      throw lock_exception();
    }
    mp_mutex->lock();
    m_locked = true;
  }

  bool try_lock()
  {
    if (!mp_mutex || m_locked)
    {
      throw lock_exception();
    }
    m_locked = mp_mutex->try_lock();
    return m_locked;
  }

  template<class TimePoint>
  bool timed_lock(const TimePoint& abs_time)
  {
    if (!mp_mutex || m_locked)
    {
      throw lock_exception();
    }
    m_locked = mp_mutex->timed_lock(abs_time);
    return m_locked;
  }

  template<class TimePoint>
  bool try_lock_until(const TimePoint& abs_time)
  {
    if (!mp_mutex || m_locked)
    {
      throw lock_exception();
    }
    m_locked = mp_mutex->try_lock_until(abs_time);
    return m_locked;
  }

  template<class Duration>
  bool try_lock_for(const Duration& dur)
  {
    if(!mp_mutex || m_locked)
      throw lock_exception();
    m_locked = mp_mutex->try_lock_for(dur);
    return m_locked;
  }

  void unlock()
  {
    if(!mp_mutex || !m_locked)
      throw lock_exception();
    mp_mutex->unlock();
    m_locked = false;
  }

  bool owns() const AUTODDS_NOEXCEPT
  {
    return m_locked && mp_mutex;
  }

  operator unspecified_bool_type() const AUTODDS_NOEXCEPT
  {
    return m_locked ? &this_type::m_locked : 0;
  }

  mutex_type* mutex() const AUTODDS_NOEXCEPT
  {
    return mp_mutex;
  }

  mutex_type * release() AUTODDS_NOEXCEPT
  {
    mutex_type* mutex = mp_mutex;
    mp_mutex = nullptr;
    m_locked = false;
    return mutex;
  }

  void swap(scoped_lock<mutex_type>& other) AUTODDS_NOEXCEPT
  {
    (simple_swap)(mp_mutex, other.mp_mutex);
    (simple_swap)(m_locked, other.m_locked);
  }

 private:
  mutex_type * mp_mutex;
  bool         m_locked;
};

} // namespace interprocess
} // namespace libs
} // namespace autodds

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_SYNC_SCOPED_LOCK_HPP_