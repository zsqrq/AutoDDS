//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {


template<bool ConstantSize, class SizeType, class Tag = void>
struct size_holder
{
  static const bool constant_time_size = ConstantSize;
  typedef SizeType  size_type;

  AUTODDS_INTRUSIVE_FORCEINLINE SizeType get_size() const
  {  return size_;  }

  AUTODDS_INTRUSIVE_FORCEINLINE void set_size(SizeType size)
  {  size_ = size; }

  AUTODDS_INTRUSIVE_FORCEINLINE void decrement()
  {  --size_; }

  AUTODDS_INTRUSIVE_FORCEINLINE void increment()
  {  ++size_; }

  AUTODDS_INTRUSIVE_FORCEINLINE void increase(SizeType n)
  {  size_ += n; }

  AUTODDS_INTRUSIVE_FORCEINLINE void decrease(SizeType n)
  {  size_ -= n; }

  AUTODDS_INTRUSIVE_FORCEINLINE void swap(size_holder &other)
  {  SizeType tmp(size_); size_ = other.size_; other.size_ = tmp; }

  SizeType size_;
};

template<class SizeType, class Tag>
struct size_holder<false, SizeType, Tag>
{
  static const bool constant_time_size = false;
  typedef SizeType  size_type;

  AUTODDS_INTRUSIVE_FORCEINLINE size_type get_size() const
  {  return 0;  }

  AUTODDS_INTRUSIVE_FORCEINLINE void set_size(size_type)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void decrement()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void increment()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void increase(SizeType)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void decrease(SizeType)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void swap(size_holder){}
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP_