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

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_MANAGED_OPEN_OR_CREATE_IMPL_HPP_