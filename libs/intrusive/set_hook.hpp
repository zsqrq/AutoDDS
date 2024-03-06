//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_SET_HOOK_HPP_
#define AUTODDS_LIBS_INTRUSIVE_SET_HOOK_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/rbtree_node.hpp"
#include "libs/intrusive/rbtree_algorithms.hpp"
#include "libs/intrusive/options.hpp"
#include "libs/intrusive/detail/generic_hook.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename ...Options>
struct make_set_base_hook
{
  typedef typename pac
};

} // namespace intrusive
} // namespace libs
} // namespace intrusive

#endif //AUTODDS_LIBS_INTRUSIVE_SET_HOOK_HPP_