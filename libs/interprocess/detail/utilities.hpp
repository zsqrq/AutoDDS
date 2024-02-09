//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/min_max.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/common/static_assert.hpp"
#include "libs/common/cstdint.hpp"
#include <climits>

namespace autodds {
namespace libs {
namespace interprocess {

template<class T>
void simple_swap(T&x, T&y)
{  T tmp(x); x = y; y = tmp;  }

namespace ipcdetail {

template <typename T>
inline T* to_raw_pointer(T* p)
{ return p;}

template <typename Ptr>
inline typename autodds::intrusive::pointer_traits<Ptr>::element_type*
to_raw_pointer(AUTODDS_LV_CONST_REF(Ptr,p))
{ return autodds::libs::interprocess::ipcdetail::to_raw_pointer(p.operator->()); }

template <typename SizeType>
inline SizeType get_rounded_size(SizeType orig_size, SizeType round_to)
{
  return ((orig_size - 1)/ round_to + 1) * round_to;
}

template <typename SizeType>
inline SizeType get_truncated_size(SizeType orig_size, SizeType multiple)
{
  return orig_size / multiple * multiple;
}

//Rounds "orig_size" by excess to round_to bytes. round_to must be power of two
template <typename SizeType>
inline SizeType get_rounded_size_po2(SizeType orig_size, SizeType round_to)
{
  return ((orig_size-1)&(~(round_to-1))) + round_to;
}

//Truncates "orig_size" to a multiple of "multiple" bytes. multiple must be power of two
template <typename SizeType>
inline SizeType get_truncated_size_po2(SizeType orig_size, SizeType multiple)
{
  return (orig_size & (~(multiple - 1)));
}

template <std::size_t OrigSize, std::size_t RoundTo>
struct ct_rounded_size
{
  AUTODDS_STATIC_ASSERT(RoundTo != 0);
  static const std::size_t intermediate_value = (OrigSize - 1) / RoundTo + 1;
  AUTODDS_STATIC_ASSERT(intermediate_value <= std::size_t(-1)/RoundTo);
  static const std::size_t value = intermediate_value*RoundTo;
};

template <std::size_t p, std::size_t n = 4>
struct ct_max_pow2_less
{
  static const std::size_t c = 2*n < p;

  static const std::size_t value =
      c ? (ct_max_pow2_less< c*p, 2*c*n>::value) : n;
};

template <>
struct ct_max_pow2_less<0, 0>
{
  static const std::size_t value = 0;
};

}

//!Trait class to detect if an index is a node
//!index. This allows more efficient operations
//!when deallocating named objects.
template <typename Index>
struct is_node_index
{
  static const bool value = false;
};

//!Trait class to detect if an index is an intrusive
//!index. This will embed the derivation hook in each
//!allocation header, to provide memory for the intrusive
//!container.
template <typename Index>
struct is_intrusive_index
{
  static const bool value = false;
};

template <typename T>
AUTODDS_INTERPROCESS_FORCEINLINE T* addressof(T& v)
{
  return reinterpret_cast<T*>(
      &const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
}

template <typename SizeType>
struct sqrt_size_type_max
{
  static const SizeType value = (SizeType(1) << (sizeof(SizeType)*(CHAR_BIT/2)))-1;
};

template <typename SizeType>
inline bool multiplication_overflows(SizeType a, SizeType b)
{
  const SizeType sqrt_size_max = sqrt_size_type_max<SizeType>::value;
  return   //Fast runtime check
      (  (a | b) > sqrt_size_max &&
          //Slow division check
          b && a > SizeType(-1)/b
      );
}

template <std::size_t SztSizeOfType, class SizeType>
AUTODDS_INTERPROCESS_FORCEINLINE bool size_overflows(SizeType count)
{
  //Compile time-check
  AUTODDS_STATIC_ASSERT(SztSizeOfType <= SizeType(-1));
  //Runtime check
  return multiplication_overflows(SizeType(SztSizeOfType), count);
}

template <typename RawPointer, typename OffsetType>
class pointer_offset_caster;

template <typename T, typename OffsetType>
class pointer_offset_caster<T*, OffsetType>
{
 public:
  AUTODDS_INTERPROCESS_FORCEINLINE explicit pointer_offset_caster(OffsetType offset_type)
      : m_offset(offset_type)
  {}

  AUTODDS_INTERPROCESS_FORCEINLINE explicit pointer_offset_caster(const volatile T* p)
      : m_offset(reinterpret_cast<OffsetType>(p))
  {}

  AUTODDS_INTERPROCESS_FORCEINLINE OffsetType offset() const
  { return m_offset; }

  AUTODDS_INTERPROCESS_FORCEINLINE T* pointer() const
  { return reinterpret_cast<T*>(m_offset); }

 private:
  OffsetType m_offset;
};

template <typename SizeType>
inline bool sum_overflows(SizeType a, SizeType b)
{  return SizeType(-1) - a < b;  }

//Anti-exception node eraser
template <typename Cont>
class value_eraser
{
 public:
  value_eraser(Cont& cont, typename Cont::iterator it)
      : m_cont(cont), m_index_it(it), m_erase(true)
  {}

  ~value_eraser()
  {
    if (m_erase) {
      m_cont.erase(m_index_it);
    }
  }

  AUTODDS_INTERPROCESS_FORCEINLINE void release()
  { m_erase = false; }

 private:
  Cont& m_cont;
  typename Cont::iterator m_index_it;
  bool m_erase;
};

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_UTILITIES_HPP_