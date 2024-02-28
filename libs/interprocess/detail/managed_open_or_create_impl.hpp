//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_OPEN_OR_CREATE_IMPL_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_OPEN_OR_CREATE_IMPL_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/os_thread_functions.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/interprocess/mapped_region.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/atomic.hpp"
#include "libs/interprocess/detail/interprocess_tester.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/permissions.hpp"
#include "libs/container/detail/type_traits.hpp"
#include "libs/interprocess/sync/spin/wait.hpp"
#include "libs/interprocess/detail/timed_utils.hpp"
#include "libs/move/move.hpp"
#include "libs/common/cstdint.hpp"

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <bool StoreDevice, typename DeviceAbstraction>
class managed_open_or_create_impl_device_holder
{
 public:
  DeviceAbstraction& get_device()
  {
    static DeviceAbstraction device;
    return device;
  }

  const DeviceAbstraction& get_device() const
  {
    static DeviceAbstraction device;
    return device;
  }
};

template <typename DeviceAbstraction>
class managed_open_or_create_impl_device_holder<true, DeviceAbstraction>
{
 public:
  DeviceAbstraction &get_device()
  {  return dev; }

  const DeviceAbstraction &get_device() const
  {  return dev; }

 private:
  DeviceAbstraction dev;
};

template <typename DeviceAbstraction, std::size_t MemAlignment, bool FileBased, bool StoreDevice>
class managed_open_or_create_impl :
    public managed_open_or_create_impl_device_holder<StoreDevice, DeviceAbstraction>
{
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(managed_open_or_create_impl)
 typedef bool_<FileBased> file_like_t;

 static const unsigned MaxCreateOrOpenTries = AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_MAX_TRIES;
 static const unsigned MaxInitializeTimeSec = AUTODDS_INTERPROCESS_MANAGED_OPEN_OR_CREATE_INITIALIZE_TIMEOUT_SEC;

 typedef managed_open_or_create_impl_device_holder<StoreDevice, DeviceAbstraction> DevHolder;

 enum
 {
   UninitializedSegment,
   InitializingSegment,
   InitializedSegment,
   CorruptedSegment
 };

 static const std::size_t RequiredAlignment = MemAlignment ? MemAlignment :
     autodds::libs::container::dtl::alignment_of<autodds::libs::container::dtl::max_align_t>::value;

 public:
  static const std::size_t ManagedOpenOrCreateUserOffset =
      ct_rounded_size<sizeof(autodds::libs::uint32_t), RequiredAlignment>::value;

  managed_open_or_create_impl()
  {}

  template <typename DeviceID>
  managed_open_or_create_impl(create_only_t, const DeviceID& id, std::size_t size,
                              mode_t mode, const void* addr, const permissions& perm)
  {
//    priv_open_or_create()
  }

 private:

  //These are templatized to allow explicit instantiations
  template <bool dummy>
  static void truncate_device(DeviceAbstraction& , offset_t , false_)
  {}

  template <bool dummy>
  static void truncate_device(DeviceAbstraction& dev, offset_t size, true_)
  { dev.truncate(size); }

  template <bool dummy>
  static bool check_offset_t_size(std::size_t, false_)
  { return true; }

  template <bool dummy>
  static bool check_offset_t_size(std::size_t size, true_)
  { return size == std::size_t(offset_t(size)); }

  //These are templatized to allow explicit instantiations
  template <bool dummy, typename DeviceId>
  static void create_device(DeviceAbstraction& dev, const DeviceId& id, std::size_t size, const permissions& perm, false_)
  {
    DeviceAbstraction tmp(create_only, id, read_write, size, perm);
    tmp.swap(dev);
  }

 private:
  friend class interprocess_tester;
  void dont_close_on_destruction()
  {
    interprocess_tester::dont_close_on_destruction(m_mapped_region);
  }
  mapped_region m_mapped_region;
};

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_OPEN_OR_CREATE_IMPL_HPP_