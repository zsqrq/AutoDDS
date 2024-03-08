//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SMART_PTR_DELETER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SMART_PTR_DELETER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/intrusive/pointer_traits.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

//!A deleter that uses the segment manager's destroy_ptr
//!function to destroy the passed pointer resource.
template<typename T, typename SegmentManager>
class deleter
{
 public:
  typedef typename autodds::intrusive::pointer_traits
      <typename SegmentManager::void_pointer>::template
          rebind_pointer<T>::type                               pointer;
 private:
  typedef typename autodds::intrusive::pointer_traits
  <pointer>::template rebind_pointer<SegmentManager>::type      segment_manager_pointer;

  segment_manager_pointer mp_manager;

 public:
  deleter(segment_manager_pointer pmngr)   AUTODDS_NOEXCEPT
   : mp_manager(pmngr)
  {}

  void operator ()(const pointer& ptr)
  {
    mp_manager->destroy_ptr(ipcdetail::to_raw_pointer(ptr));
  }
};

} // namespace interprocess
} // namespace libs
} // namespace autodds

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_SMART_PTR_DELETER_HPP_