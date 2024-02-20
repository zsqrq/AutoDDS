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

////////////////////////////////////////////////////////////////////////
//                      offset_ptr_to_offset_from_other
////////////////////////////////////////////////////////////////////////
#define AUTODDS_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF_FROM_OTHER
template <typename OffsetType>
AUTODDS_INTERPROCESS_FORCEINLINE OffsetType offset_ptr_to_offset_from_other
    (const volatile void *this_ptr, const volatile void *other_ptr, OffsetType other_offset)
{
  typedef pointer_offset_caster<void*, OffsetType> caster_t;
  OffsetType mask = other_offset == 1;
  --mask;
  OffsetType offset = caster_t(other_ptr).offset() - caster_t(this_ptr).offset();
  offset &= mask;
  return offset + other_offset;
}

template <typename From, typename To>
struct offset_ptr_maintains_address
{
  static const bool value = ipcdetail::is_cv_same<From, To>::value ||
      ipcdetail::is_cv_same<void, To>::value ||
      ipcdetail::is_cv_same<char, To>::value;
};

template <typename From, typename To, typename Ret = void>
struct enable_if_convertible_equal_address
    : enable_if_c<autodds::libs::is_convertible<From*,To*>::value &&
        offset_ptr_maintains_address<From, To>::value, Ret>
{};

template<class From, class To, class Ret = void>
struct enable_if_convertible_unequal_address
    : enable_if_c< ::autodds::libs::is_convertible<From*, To*>::value
                       && !offset_ptr_maintains_address<From, To>::value
        , Ret>
{};

} // namespace ipcdetail

//!A smart pointer that stores the offset between between the pointer and the
//!the object it points. This allows offset allows special properties, since
//!the pointer is independent from the address of the pointee, if the
//!pointer and the pointee are still separated by the same offset. This feature
//!converts offset_ptr in a smart pointer that can be placed in shared memory and
//!memory mapped files mapped in different addresses in every process.
//!
template <typename PointedType, typename DifferenceType, typename OffsetType, std::size_t OffsetAlignment>
class offset_ptr
{
  typedef offset_ptr<PointedType,DifferenceType,OffsetType,OffsetAlignment>   self_t;
  void unspecified_bool_type_func() const {}
  typedef void (self_t::*unspecified_bool_type)() const;

 public:
  typedef PointedType                                                   element_type;
  typedef PointedType*                                                  pointer;
  typedef  typename ipcdetail::add_reference<PointedType>::type         reference;
  typedef typename ipcdetail::remove_volatile<
      typename ipcdetail::remove_const<PointedType>::type>::type        value_type;
  typedef DifferenceType                                                difference_type;
  typedef std::random_access_iterator_tag                               iterator_category;
  typedef OffsetType                                                    offset_type;

 public:
  //!Default constructor (null pointer).
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr() AUTODDS_NOEXCEPT
      : internal(1)
  {}

  //!Constructor from raw pointer (allows "0" pointer conversion).
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(pointer ptr) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(ptr, this))
  {}

  //!Constructor from other pointer.
  template<typename T>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr(T* ptr,typename ipcdetail::enable_if<is_convertible<T*, PointedType*>>::type * = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType*>(ptr), this))
  {}

  //!Constructor from other offset_ptr
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const OffsetType& ptr) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.internal.m_offset))
  {}

  //!Constructor from other offset_ptr. Only takes part in overload resolution
  //!if T2* is convertible to PointedType*. Never throws.
  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment>& ptr,
             typename ipcdetail::enable_if_convertible_equal_address<T2, PointedType>::type* =0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.get_offset()))
  {}

  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr( const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr,
              typename ipcdetail::enable_if_convertible_unequal_address<T2, PointedType>::type* = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType*>(ptr.get()), this))
  {}

  //!Constructor from other offset_ptr. Only takes part in overload resolution
  //!if PointedType* is constructible from T2* other than via a conversion
  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  explicit offset_ptr(const offset_ptr<T2,DifferenceType,OffsetType,OffsetAlignment>& ptr,
                      typename ipcdetail::enable_if_c<!is_convertible<T2*,PointedType*>::value &&
                      autodds::libs::is_con>)


 private:
  ipcdetail::offset_ptr_internal<OffsetType, OffsetAlignment> internal;

 public:
  AUTODDS_INTERPROCESS_FORCEINLINE const OffsetType& priv_offset() const AUTODDS_NOEXCEPT
  { return internal.m_offset; }

  AUTODDS_INTERPROCESS_FORCEINLINE OffsetType& priv_offset() AUTODDS_NOEXCEPT
  { return internal.m_offset; }
};


} // namespace interprocess
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_