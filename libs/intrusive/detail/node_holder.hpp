//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_NODE_HOLDER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_NODE_HOLDER_HPP_

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

template<typename Node, typename Tag, unsigned int>
struct node_holder
    :  public Node
{};


} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_NODE_HOLDER_HPP_