//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_
#define AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_

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
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/detail/cast_tags.hpp"
#include "libs/interprocess/detail/mpl.hpp"

#include "libs/type_traits/is_convertible.hpp"
#include "libs/type_traits/is_constructible.hpp"
#include "libs/type_traits/is_integral.hpp"
#include "libs/type_traits/is_unsigned.hpp"

#include "libs/container/detail/type_traits.hpp"
#include "libs/common/assert.hpp"
#include "libs/common/static_assert.hpp"
#include <iosfwd>

#if defined(AUTODDS_GCC) && (AUTODDS_GCC >= 40600)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

namespace autodds {
namespace libs {

template <typename T>
struct has_trival_destructor;

namespace interprocess {

namespace ipcdetail {

template <typename OffsetType, std::size_t OffsetAlignment>
union offset_ptr_internal
{
  AUTODDS_STATIC_ASSERT(sizeof(OffsetType) >= sizeof(uintptr_t));
  AUTODDS_STATIC_ASSERT(autodds::libs::is_integral<OffsetType>::value && autodds::libs::is_unsigned<OffsetType>::value);

  explicit offset_ptr_internal(OffsetType off)
      : m_offset(off)
  {}

  OffsetType m_offset;

  typename autodds::libs::move_detail::aligned_storage<
      sizeof(OffsetType),(OffsetAlignment == offset_type_alignment) ? 1u : OffsetAlignment>::type alignment_helper;
};

////////////////////////////////////////////////////////////////////////
//                      offset_ptr_to_raw_pointer
////////////////////////////////////////////////////////////////////////
#define AUTODDS_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_PTR
template <typename OffsetType>
AUTODDS_INTERPROCESS_FORCEINLINE void* offset_ptr_to_raw_pointer(const volatile void* this_ptr, OffsetType offset)
{
  typedef pointer_offset_caster<void*, OffsetType> caster_t;
  OffsetType mask = offset == 1;
  --mask;
  OffsetType target_offset = caster_t(this_ptr).offset() + offset;
  target_offset &= mask;
  return caster_t(target_offset).pointer();
}

////////////////////////////////////////////////////////////////////////
//                      offset_ptr_to_offset
////////////////////////////////////////////////////////////////////////
#define AUTODDS_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF
template <typename OffsetType>
AUTODDS_INTERPROCESS_FORCEINLINE OffsetType offset_ptr_to_offset(const volatile void* ptr, const volatile void* this_ptr)
{
  typedef pointer_offset_caster<void*, OffsetType> caster_t;
  OffsetType offset = caster_t(ptr).offset() - caster_t(this_ptr).offset();
  --offset;
  OffsetType mask = ptr == 0;
  --mask;
  offset &= mask;
  return ++offset;
}

}

}


}
}


#endif //AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_