//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/move/detail/to_raw_pointer.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

// trivial header node holder
template < typename NodeTraits >
struct default_header_holder : public NodeTraits::node
{
  typedef NodeTraits node_traits;
  typedef typename node_traits::node node;
  typedef typename node_traits::node_ptr node_ptr;
  typedef typename node_traits::const_node_ptr const_node_ptr;

  default_header_holder() : node() {}

  AUTODDS_INTRUSIVE_FORCEINLINE const_node_ptr get_node() const
  { return pointer_traits< const_node_ptr >::pointer_to(*static_cast< const node* >(this)); }

  AUTODDS_INTRUSIVE_FORCEINLINE node_ptr get_node()
  { return pointer_traits< node_ptr >::pointer_to(*static_cast< node* >(this)); }

  // (unsafe) downcast used to implement container-from-iterator
  AUTODDS_INTRUSIVE_FORCEINLINE static default_header_holder* get_holder(node_ptr p)
  { return static_cast< default_header_holder* >(autodds::libs::movelib::to_raw_pointer(p)); }
};

// type function producing the header node holder
template < typename ValueTraits, typename HeaderHolder >
struct get_header_holder_type
{
  typedef HeaderHolder type;
};
template < typename ValueTraits >
struct get_header_holder_type< ValueTraits, void >
{
  typedef default_header_holder< typename ValueTraits::node_traits > type;
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP_