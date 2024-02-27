//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_TIMED_UTILS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_TIMED_UTILS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/common/cstdint.hpp"

#include <ctime>
#ifdef AUTODDS_HAS_GETTIMEOFDAY
#include <sys/time.h>
#endif

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(time_duration_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(clock)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(rep_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(rep)

template <typename T>
struct enable_if_ptime :
    public enable_if_c<AUTODDS_INTRUSIVE_HAS_TYPE(autodds::libs::interprocess::ipcdetail::, T, time_duration_type)>
{};

template <typename T>
struct disable_if_ptime
    : public enable_if_c<!AUTODDS_INTRUSIVE_HAS_TYPE(autodds::libs::interprocess::ipcdetail::, T, time_duration_type)>
{};

template <typename T>
struct enable_if_ptime_duration :
    public enable_if_c<AUTODDS_INTRUSIVE_HAS_TYPE(autodds::libs::interprocess::ipcdetail::, T, rep_type)>
{};

template <typename T>
struct enable_if_time_point :
    public enable_if_c<AUTODDS_INTRUSIVE_HAS_TYPE(autodds::libs::interprocess::ipcdetail::, T, clock)>
{};

template <typename T>
struct enable_if_duration :
    public enable_if_c<AUTODDS_INTRUSIVE_HAS_TYPE(autodds::libs::interprocess::ipcdetail::, T, rep)>
{};

#if defined(AUTODDS_INTERPROCESS_HAS_REENTRANT_STD_FUNCTIONS)

inline std::tm* interprocess_gmtime(const std::time_t* t, std::tm* result)
{
  result = gmtime_r(t, result);
  return result;
}

#endif

class ustime;

class usduration
{
 public:
  friend class ustime;

  explicit usduration(autodds::libs::uint64_t microseconds)
      : m_microsecs(microseconds)
  {}

  autodds::libs::uint64_t get_microsecs() const
  { return m_microsecs; }

  bool operator < (const usduration& other) const
  { return m_microsecs < other.m_microsecs; }

  bool operator > (const usduration& other) const
  { return m_microsecs > other.m_microsecs; }

  bool operator <= (const usduration& other) const
  { return m_microsecs <= other.m_microsecs; }

  bool operator >= (const usduration& other) const
  { return m_microsecs >= other.m_microsecs; }

 private:
  autodds::libs::uint64_t m_microsecs;
};

class ustime
{
 public:
  explicit ustime(autodds::libs::uint64_t microseconds)
    : m_microsecs(microseconds)
  {}

  ustime& operator += (const usduration& other)
  {
    m_microsecs += other.m_microsecs;
    return *this;
  }

  ustime operator + (const usduration& other)
  {
    ustime tmp(*this);
    tmp += other;
    return tmp;
  }

  ustime& operator -= (const usduration& other)
  {
    m_microsecs -= other.m_microsecs;
    return *this;
  }

  ustime operator - (const usduration& other)
  {
    ustime tmp(*this);
    tmp -= other;
    return tmp;
  }

  friend usduration operator - (const ustime& left, const ustime& right)
  { return usduration(left.m_microsecs - right.m_microsecs); }

  bool operator < (const ustime& other) const
  { return m_microsecs < other.m_microsecs; }

  bool operator > (const ustime& other) const
  { return m_microsecs > other.m_microsecs; }

  bool operator <= (const ustime &other) const
  {  return m_microsecs <= other.m_microsecs; }

  bool operator >= (const ustime &other) const
  {  return m_microsecs >= other.m_microsecs; }

  autodds::libs::uint64_t get_microsecs() const
  { return m_microsecs; }
 private:
  autodds::libs::uint64_t m_microsecs;
};

inline usduration usduration_milliseconds(autodds::libs::uint64_t millisec)
{ return usduration(millisec * 1000u); }

inline usduration usduration_seconds(autodds::libs::uint64_t sec)
{ return usduration(sec * uint64_t(1000000u)); }

template <typename TimeType, typename Enable = void>
class microsec_clock;

template <typename TimeType>
class microsec_clock<TimeType, typename enable_if_ptime<TimeType>::type>
{
 private:
  typedef typename TimeType::date_type date_type;
  typedef typename TimeType::time_duration_type time_duration_type;
  typedef typename time_duration_type::rep_type resolutiom_traits_type;

  static TimeType universal_time()
  {
#ifdef AUTODDS_HAS_GETTIMEOFDAY
    timeval tv;
    gettimeofday(&tv,0);
    std::time_t t = tv.tv_sec;
    autodds::libs::uint32_t sub_sec = static_cast<autodds::libs::uint32_t>(tv.tv_usec);
#endif

    std::tm curr;
    std::tm* curr_ptr = interprocess_gmtime(&t, &curr);
    date_type d(static_cast<typename date_type::year_type::value_type>(curr_ptr->tm_year + 1900),
                static_cast<typename date_type::month_type::value_type>(curr_ptr->tm_mon + 1),
                static_cast<typename date_type::day_type::value_type>(curr_ptr->tm_mday));

    unsigned adjust = static_cast<unsigned >(resolutiom_traits_type::res_adjust() / 1000000);

    time_duration_type  td(static_cast< typename time_duration_type::hour_type >(curr_ptr->tm_hour),
                           static_cast< typename time_duration_type::min_type >(curr_ptr->tm_min),
                           static_cast< typename time_duration_type::sec_type >(curr_ptr->tm_sec),
                           static_cast< typename time_duration_type::fractional_seconds_type >(sub_sec * adjust));
    return TimeType(d, td);
  }
};

template<>
class microsec_clock<ustime>
{
 public:
  static ustime universal_time()
  {
#ifdef AUTODDS_HAS_GETTIMEOFDAY
    timeval tv;
    gettimeofday(&tv, 0);
    autodds::libs::uint64_t micros = autodds::libs::uint64_t(tv.tv_sec) * 1000000;
    micros += (autodds::libs::uint64_t)tv.tv_usec;
#endif
    return ustime(micros);
  }
};

template<typename TimePoint>
class microsec_clock<TimePoint, typename enable_if_time_point<TimePoint>::type>
{
 public:
  static TimePoint universal_time()
  {  return TimePoint::clock::now();  }
};

template <typename TimePoint>
inline TimePoint delay_ms(unsigned msecs, typename enable_if_ptime<TimePoint>::type* = 0)
{
  typedef typename TimePoint::time_duration_type time_duration_type;
  typedef typename time_duration_type::rep_type resolution_traits_type;

  time_duration_type td(msecs * 1000 * resolution_traits_type::res_adjust());
  TimePoint tp(microsec_clock<TimePoint>::universal_time());
  return (tp += td);
}

template<typename TimePoint>
inline bool is_pos_infinity(const TimePoint &abs_time, typename enable_if_ptime<TimePoint>::type* = 0)
{
  return abs_time.is_pos_infinity();
}

template<typename TimePoint>
inline bool is_pos_infinity(const TimePoint &, typename disable_if_ptime<TimePoint>::type* = 0)
{
  return false;
}

template<typename Duration>
inline autodds::libs::uint64_t duration_to_milliseconds(const Duration &abs_time,
                                                        typename enable_if_ptime_duration<Duration>::type* = 0)
{
  return static_cast<autodds::libs::uint64_t>(abs_time.total_milliseconds());
}

template<typename Duration>
inline autodds::libs::uint64_t duration_to_milliseconds(const Duration &d,
                                                        typename enable_if_duration<Duration>::type* = 0)
{
  const double factor = double(Duration::period::num) * 1000.0 / double(Duration::period::den);
  return static_cast<autodds::libs::uint64_t>(double(d.count()) * factor);
}

inline autodds::libs::uint64_t duration_to_milliseconds(const usduration& dura)
{
  return dura.get_microsecs() / 1000;
}

template<typename Duration>
inline usduration duration_to_usduration(const Duration &d, typename enable_if_ptime_duration<Duration>::type* = 0)
{
  return usduration(static_cast<autodds::libs::uint64_t>(d.total_microseconds()));
}

template<typename Duration>
inline usduration duration_to_usduration(const Duration &d, typename enable_if_duration<Duration>::type* = 0)
{
  const double factor = double(Duration::period::num)*1000000.0/double(Duration::period::den);
  return usduration(static_cast<autodds::libs::uint64_t>(double(d.count())*factor));
}

template<typename Duration>
inline ustime duration_to_ustime(const Duration &d)
{
  return microsec_clock<ustime>::universal_time() + (duration_to_usduration)(d);
}

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_TIMED_UTILS_HPP_