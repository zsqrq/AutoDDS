//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/function_detector.hpp"

AUTODDS_INTRUSIVE_CREATE_FUNCTION_DETECTOR(to_node_ptr, autodds_libs_intrusive)
AUTODDS_INTRUSIVE_CREATE_FUNCTION_DETECTOR(to_value_ptr, autodds_libs_intrusive)

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<class ValueTraits>
struct is_stateful_value_traits
{
  typedef typename ValueTraits::node_ptr       node_ptr;
  typedef typename ValueTraits::pointer        pointer;
  typedef typename ValueTraits::value_type     value_type;
  typedef typename ValueTraits::const_node_ptr const_node_ptr;
  typedef typename ValueTraits::const_pointer  const_pointer;

  typedef ValueTraits value_traits;

  static const bool value =
      (autodds::libs::intrusive::function_detector::NonStaticFunction ==
          (AUTODDS_INTRUSIVE_DETECT_FUNCTION(ValueTraits, autodds_libs_intrusive, node_ptr, to_node_ptr, (value_type&) )))
          ||
              (autodds::libs::intrusive::function_detector::NonStaticFunction ==
                  (AUTODDS_INTRUSIVE_DETECT_FUNCTION(ValueTraits, autodds_libs_intrusive, pointer, to_value_ptr, (node_ptr) )))
          ||
              (autodds::libs::intrusive::function_detector::NonStaticFunction ==
                  (AUTODDS_INTRUSIVE_DETECT_FUNCTION(ValueTraits, autodds_libs_intrusive, const_node_ptr, to_node_ptr, (const value_type&) )))
  ||
  (autodds::libs::intrusive::function_detector::NonStaticFunction ==
  (AUTODDS_INTRUSIVE_DETECT_FUNCTION(ValueTraits, autodds_libs_intrusive, const_pointer, to_value_ptr, (const_node_ptr) )))
  ;
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP_