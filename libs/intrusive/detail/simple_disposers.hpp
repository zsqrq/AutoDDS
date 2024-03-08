//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP_

#include "libs/intrusive/detail/workaround.hpp"

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

class null_disposer
{
 public:
  template <class Pointer>
  void operator()(Pointer)
  {}
};

template<class NodeAlgorithms>
class init_disposer
{
  typedef typename NodeAlgorithms::node_ptr node_ptr;

 public:
  AUTODDS_INTRUSIVE_FORCEINLINE void operator()(node_ptr p)
  {  NodeAlgorithms::init(p);   }
};


} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_SIMPLE_DISPOSERS_HPP_