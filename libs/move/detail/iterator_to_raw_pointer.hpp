//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TO_RAW_POINTER_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TO_RAW_POINTER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/iterator_traits.hpp"
#include "libs/move/detail/to_raw_pointer.hpp"
#include "libs/move/detail/pointer_element.hpp"

namespace autodds {
namespace libs {
namespace movelib {
namespace detail {

template <typename T>
AUTODDS_MOVE_FORCEINLINE T* iterator_to_pointer(T* p)
{ return p; }

template <typename Iterator>
AUTODDS_MOVE_FORCEINLINE typename autodds::libs::movelib::iterator_traits<Iterator>::pointer
iterator_to_pointer(const Iterator& i)
{ return i.operator->(); }

template <typename Iterator>
struct iterator_to_element_ptr
{
  typedef typename autodds::libs::movelib::iterator_traits<Iterator>::pointer   pointer;
  typedef typename autodds::movelib::pointer_element<pointer>::type             element_type;
  typedef element_type * type;
};

} // namespace detail

template <typename Iterator>
AUTODDS_MOVE_FORCEINLINE typename detail::iterator_to_element_ptr<Iterator>::type
iterator_to_raw_pointer(const Iterator& i)
{
  return autodds::libs::movelib::to_raw_pointer(detail::iterator_to_pointer(i));
}

} // namespace movelib
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TO_RAW_POINTER_HPP_