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
struct has_trivial_destructor;

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
class offset_ptr {
  typedef offset_ptr<PointedType, DifferenceType, OffsetType, OffsetAlignment> self_t;
  void unspecified_bool_type_func() const {}
  typedef void (self_t::*unspecified_bool_type)() const;

 public:
  typedef PointedType element_type;
  typedef PointedType *pointer;
  typedef typename ipcdetail::add_reference<PointedType>::type reference;
  typedef typename ipcdetail::remove_volatile<
      typename ipcdetail::remove_const<PointedType>::type>::type value_type;
  typedef DifferenceType difference_type;
  typedef std::random_access_iterator_tag iterator_category;
  typedef OffsetType offset_type;

 public:
  //!Default constructor (null pointer).
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr() AUTODDS_NOEXCEPT
      : internal(1) {}

  //!Constructor from raw pointer (allows "0" pointer conversion).
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(pointer ptr) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(ptr, this)) {}

  //!Constructor from other pointer.
  template<typename T>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr(T *ptr, typename ipcdetail::enable_if<is_convertible<T *, PointedType *>>::type * = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType *>(ptr), this)) {}

  //!Constructor from other offset_ptr
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const OffsetType &ptr) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.internal.m_offset)) {}

  //!Constructor from other offset_ptr. Only takes part in overload resolution
  //!if T2* is convertible to PointedType*. Never throws.
  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr,
             typename ipcdetail::enable_if_convertible_equal_address<T2, PointedType>::type * = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.get_offset())) {}

  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  offset_ptr(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr,
             typename ipcdetail::enable_if_convertible_unequal_address<T2, PointedType>::type * = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType *>(ptr.get()), this)) {}

  //!Constructor from other offset_ptr. Only takes part in overload resolution
  //!if PointedType* is constructible from T2* other than via a conversion
  template<typename T2>
  AUTODDS_INTERPROCESS_FORCEINLINE
  explicit offset_ptr(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr,
                      typename ipcdetail::enable_if_c<!is_convertible<T2 *, PointedType *>::value &&
                          autodds::libs::is_constructible<T2 *, PointedType>::value>::type * = 0) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType>(ptr.get()), this)) {}

  //!Emulates static_cast operator.
  template<typename T1, typename T2, typename T3, std::size_t N>
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T1, T2, T3, N> &ptr,
                                              ::autodds::libs::interprocess::ipcdetail::static_cast_tag) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType *>(ptr.get()), this)) {}

  //!Emulates const_cast operator.
  template<typename T1, typename T2, typename T3, std::size_t N>
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T1, T2, T3, N> &ptr,
                                              ::autodds::libs::interprocess::ipcdetail::const_cast_tag) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(const_cast<PointedType *>(ptr.get()), this)) {}

  //!Emulates dynamic_cast operator.
  template<typename T1, typename T2, typename T3, std::size_t N>
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T1, T2, T3, N> &ptr,
                                              ::autodds::libs::interprocess::ipcdetail::dynamic_cast_tag) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(dynamic_cast<PointedType *>(ptr.get()), this)) {}

  //!Emulates reinterpret_cast operator.
  template<typename T1, typename T2, typename T3, std::size_t N>
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T1, T2, T3, N> &ptr,
                                              ::autodds::libs::interprocess::ipcdetail::reinterpret_cast_tag) AUTODDS_NOEXCEPT
      : internal(ipcdetail::offset_ptr_to_offset<OffsetType>(reinterpret_cast<PointedType *>(ptr.get()), this)) {}

  //!Obtains raw pointer from offset.
  AUTODDS_INTERPROCESS_FORCEINLINE pointer get() const AUTODDS_NOEXCEPT {
    return static_cast<pointer>(ipcdetail::offset_ptr_to_raw_pointer(this,
                                                                     this->internal.m_offset));
  }

  AUTODDS_INTERPROCESS_FORCEINLINE offset_type get_offset() const AUTODDS_NOEXCEPT { return this->internal.m_offset; }

  //! pointer like operator
  AUTODDS_INTERPROCESS_FORCEINLINE pointer operator->() const AUTODDS_NOEXCEPT { return this->get(); }

  //! Dereference operator
  AUTODDS_INTERPROCESS_FORCEINLINE reference operator*() const AUTODDS_NOEXCEPT {
    pointer p = this->get();
    reference r = *p;
    return r;
  }

  //! Indexing operator
  AUTODDS_INTERPROCESS_FORCEINLINE reference operator[](difference_type idx) const AUTODDS_NOEXCEPT { return this->get()[idx]; }

  //! Assignment from pointer
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator=(pointer from) AUTODDS_NOEXCEPT {
    this->internal.m_offset = ipcdetail::offset_ptr_to_offset<OffsetType>(from, this);
    return *this;
  }

  //! Assignment from other offset_ptr
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator=(const offset_ptr &ptr) AUTODDS_NOEXCEPT {
    this->internal.m_offset = ipcdetail::offset_ptr_to_offset_from_other(this, ptr, ptr.internal.m_offset);
    return *this;
  }

  //!Assignment from related offset_ptr.
  template<typename RLT>
  AUTODDS_INTERPROCESS_FORCEINLINE
  typename ipcdetail::enable_if_c<autodds::libs::is_convertible<RLT *, PointedType *>::value, offset_ptr &>::type
  operator=(const offset_ptr<RLT, DifferenceType, OffsetType, OffsetAlignment> &ptr) AUTODDS_NOEXCEPT {
    this->assign(ptr, ipcdetail::bool_<ipcdetail::offset_ptr_maintains_address<RLT, PointedType>::value>());
    return *this;
  }

 public:
  //! += operator
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator+=(difference_type offset) AUTODDS_NOEXCEPT {
    this->inc_offset(offset * difference_type(sizeof(PointedType)));
    return *this;
  }

  //! -+ operator
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator-=(difference_type offset) AUTODDS_NOEXCEPT {
    this->dec_offset(offset * difference_type(sizeof(PointedType)));
    return *this;
  }

  //! ++ operatior
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator++(void) AUTODDS_NOEXCEPT {
    this->inc_offset(difference_type(sizeof(PointedType)));
    return *this;
  }

  //! operator ++
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr operator++(int) AUTODDS_NOEXCEPT {
    offset_ptr tmp(*this);
    this->inc_offset(sizeof(PointedType));
    return tmp;
  }

  //！ -- operator
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr &operator--(void) AUTODDS_NOEXCEPT {
    this->dec_offset(sizeof(PointedType));
    return *this;
  }

  //！ operator --
  AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr operator--(int) AUTODDS_NOEXCEPT {
    offset_ptr tmp(*this);
    this->dec_offset(sizeof(PointedType));
    return tmp;
  }

  //! safe bool conversion
  explicit operator bool() const AUTODDS_NOEXCEPT {
    return this->internal.m_offset != 1;
  }

  //! Not operator
  AUTODDS_INTERPROCESS_FORCEINLINE bool operator!() const AUTODDS_NOEXCEPT {
    return this->internal.m_offset == 1;
  }

  template<typename NewType>
  using rebing = offset_ptr<NewType, DifferenceType, OffsetType, OffsetAlignment>;

  typedef offset_ptr<PointedType, DifferenceType, OffsetType, OffsetAlignment> other;

  //!Compatibility with pointer_traits
  AUTODDS_INTERPROCESS_FORCEINLINE static offset_ptr pointer_to(reference r) AUTODDS_NOEXCEPT { return offset_ptr(&r); }

  //!difference_type + offset_ptr
  AUTODDS_INTERPROCESS_FORCEINLINE friend offset_ptr operator+(difference_type diff,
                                                               offset_ptr right) AUTODDS_NOEXCEPT {
    right += diff;
    return right;
  }

  //!offset_ptr + difference_type
  AUTODDS_INTERPROCESS_FORCEINLINE friend offset_ptr operator+(offset_ptr left, difference_type diff) AUTODDS_NOEXCEPT {
    left += diff;
    return left;
  }

  //!offset_ptr - diff
  AUTODDS_INTERPROCESS_FORCEINLINE friend offset_ptr operator-(offset_ptr left, difference_type diff) AUTODDS_NOEXCEPT {
    left -= diff;
    return left;
  }

  //!offset_ptr - diff
  AUTODDS_INTERPROCESS_FORCEINLINE friend offset_ptr operator-(difference_type diff,
                                                               offset_ptr right) AUTODDS_NOEXCEPT {
    right -= diff;
    return right;
  }

  //!offset_ptr - offset_ptr
  AUTODDS_INTERPROCESS_FORCEINLINE friend difference_type operator-(const offset_ptr &ptr1,
                                                                    const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return difference_type(ptr1.get() - ptr2.get());
  }

  //! Comparision
  AUTODDS_INTERPROCESS_FORCEINLINE friend difference_type operator==(const offset_ptr &ptr1,
                                                                     const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() == ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator!=(const offset_ptr &ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() != ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<(const offset_ptr &ptr1,
                                                         const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() < ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<=(const offset_ptr &ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() <= ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>(const offset_ptr &ptr1,
                                                         const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() > ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>=(const offset_ptr &ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() >= ptr2.get();
  }

  //Comparison to raw ptr to support literal 0
  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator==(pointer ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 == ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator!=(pointer ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 != ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<(pointer ptr1, const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 < ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<=(pointer ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 <= ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>(pointer ptr1, const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 > ptr2.get();
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>=(pointer ptr1,
                                                          const offset_ptr &ptr2) AUTODDS_NOEXCEPT {
    return ptr1 >= ptr2.get();
  }

  //Comparison
  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator==(const offset_ptr &ptr1,
                                                          pointer ptr2) AUTODDS_NOEXCEPT { return ptr1.get() == ptr2; }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator!=(const offset_ptr &ptr1,
                                                          pointer ptr2) AUTODDS_NOEXCEPT { return ptr1.get() != ptr2; }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<(const offset_ptr &ptr1, pointer ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() < ptr2;
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator<=(const offset_ptr &ptr1,
                                                          pointer ptr2) AUTODDS_NOEXCEPT { return ptr1.get() <= ptr2; }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>(const offset_ptr &ptr1, pointer ptr2) AUTODDS_NOEXCEPT {
    return ptr1.get() > ptr2;
  }

  AUTODDS_INTERPROCESS_FORCEINLINE friend bool operator>=(const offset_ptr &ptr1,
                                                          pointer ptr2) AUTODDS_NOEXCEPT { return ptr1.get() >= ptr2; }

  AUTODDS_INTERPROCESS_FORCEINLINE friend void swap(offset_ptr &left, offset_ptr &right) AUTODDS_NOEXCEPT {
    pointer tmp = right.get();
    right = left;
    left = tmp;
  }

 private:
  template<typename D>
  AUTODDS_INTERPROCESS_FORCEINLINE void assign(const offset_ptr<D, DifferenceType, OffsetType, OffsetAlignment> &ptr,
                                               ipcdetail::bool_<true>) AUTODDS_NOEXCEPT {
    this->internal.m_offset = ipcdetail::offset_ptr_to_offset_from_other<OffsetType>(this, &ptr, ptr.get_offset());
  }

  template<typename D>
  AUTODDS_INTERPROCESS_FORCEINLINE void assign(const offset_ptr<D, DifferenceType, OffsetType, OffsetAlignment> &ptr,
                                               ipcdetail::bool_<false>) AUTODDS_NOEXCEPT {
    this->internal.m_offset = ipcdetail::offset_ptr_to_offset<OffsetType>(static_cast<PointedType *>(ptr.get()), this);
  }

  AUTODDS_INTERPROCESS_FORCEINLINE void inc_offset(DifferenceType bytes) AUTODDS_NOEXCEPT
  {
    internal.m_offset += OffsetType(bytes);
  }

  AUTODDS_INTERPROCESS_FORCEINLINE void dec_offset(DifferenceType bytes) AUTODDS_NOEXCEPT
  {
    internal.m_offset -= OffsetType(bytes);
  }

 private:
  ipcdetail::offset_ptr_internal<OffsetType, OffsetAlignment> internal;

 public:
  AUTODDS_INTERPROCESS_FORCEINLINE const OffsetType& priv_offset() const AUTODDS_NOEXCEPT
  { return internal.m_offset; }

  AUTODDS_INTERPROCESS_FORCEINLINE OffsetType& priv_offset() AUTODDS_NOEXCEPT
  { return internal.m_offset; }
};

//! operator << for offset ptr
template<typename T1, typename T2, typename T3, typename T4, typename T5, std::size_t N>
inline std::basic_ostream<T1, T2>& operator << (std::basic_ostream<T1, T2>& os, offset_ptr<T3, T4, T5, N> const& p)
{  return os << p.get_offset();   }

//! operator >> for offset ptr
template<typename T1, typename T2, typename T3, typename T4, typename T5, std::size_t N>
inline std::basic_ostream<T1, T2>& operator >> (std::basic_ostream<T1, T2>& is, offset_ptr<T3, T4, T5, N> const& p)
{  return is >> p.get_offset();   }

//! Simulation of static_cast between pointers. Never throws.
template <typename T1, typename P1, typename O1, std::size_t N1, typename T2, typename P2, typename O2, std::size_t N2>
AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr<T1, P1, O1, N1> static_pointer_cast(const offset_ptr<T2, P2, O2, N2>& ptr2) AUTODDS_NOEXCEPT
{
  return offset_ptr<T1, P1, O1, N1>(ptr2, autodds::libs::interprocess::ipcdetail::static_cast_tag());
}

//!Simulation of const_cast between pointers. Never throws.
template <typename T1, typename P1, typename O1, std::size_t N1, typename T2, typename P2, typename O2, std::size_t N2>
AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr<T1, P1, O1, N1> const_pointer_cast(const offset_ptr<T2, P2, O2, N2>& ptr2) AUTODDS_NOEXCEPT
{
  return offset_ptr<T1, P1, O1, N1>(ptr2, autodds::libs::interprocess::ipcdetail::const_cast_tag());
}

//!Simulation of dynamic_cast between pointers. Never throws.
template <typename T1, typename P1, typename O1, std::size_t N1, typename T2, typename P2, typename O2, std::size_t N2>
AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr<T1, P1, O1, N1> dynamic_pointer_cast(const offset_ptr<T2, P2, O2, N2>& ptr2) AUTODDS_NOEXCEPT
{
  return offset_ptr<T1, P1, O1, N1>(ptr2, autodds::libs::interprocess::ipcdetail::dynamic_cast_tag());
}

//!Simulation of reinterpret_cast between pointers. Never throws.
template <typename T1, typename P1, typename O1, std::size_t N1, typename T2, typename P2, typename O2, std::size_t N2>
AUTODDS_INTERPROCESS_FORCEINLINE offset_ptr<T1, P1, O1, N1> reinterpret_pointer_cast(const offset_ptr<T2, P2, O2, N2>& ptr2) AUTODDS_NOEXCEPT
{
  return offset_ptr<T1, P1, O1, N1>(ptr2, autodds::libs::interprocess::ipcdetail::reinterpret_cast_tag());
}

} // namespace interprocess

///has_trivial_destructor<> == true_type specialization for optimizations
template <typename T, typename P, typename O, std::size_t N>
struct has_trivial_destructor<interprocess::offset_ptr<T, P, O, N>>
{
  static const bool value = true;
};

namespace move_detail {
///has_trivial_destructor<> == true_type specialization for optimizations
template <typename T, typename P, typename O, std::size_t N>
struct is_trivially_destructible<::autodds::libs::interprocess::offset_ptr<T, P, O, N>>
{
  static const bool value = true;
};
} // namespace move_detail

namespace interprocess {

//!to_raw_pointer()
//!Never throws.
template <typename T, typename P, typename O, std::size_t N>
AUTODDS_INTERPROCESS_FORCEINLINE T * to_raw_pointer(::autodds::libs::interprocess::offset_ptr<T, P, O, N> const & p) AUTODDS_NOEXCEPT
{  return ipcdetail::to_raw_pointer(p);   }
}  //namespace interprocess

} // namespace libs
} // namespace autodds

namespace autodds {
namespace libs {
namespace intrusive {

template<typename VoidPointer, std::size_t N>
struct max_pointer_plus_bits;

template<std::size_t OffsetAlignment, class P, class O, std::size_t A>
struct max_pointer_plus_bits<autodds::libs::interprocess::offset_ptr<void, P, O, A>, OffsetAlignment>
{
//The offset ptr can embed one bit less than the alignment since it
//uses offset == 1 to store the null pointer.
static const std::size_t value = ::autodds::libs::interprocess::ipcdetail::ls_zeros<OffsetAlignment>::value - 1;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_INTERPROCESS_OFFSET_PTR_HPP_