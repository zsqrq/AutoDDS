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
  typedef typename pack_options<hook_defaults,Options...>::type                    packed_options;
  typedef generic_hook<RbTreeAlgorithms,
                       rbtree_node_traits<typename packed_options::void_pointer,packed_options::optimize_size>,
                       typename packed_options::tag, packed_options::link_mode,
                       RbTreeBaseHookId>                                           implementation_defined;
  typedef implementation_defined                                                   type;
};

template<typename ...Options>
class set_base_hook : public make_set_base_hook<Options...>::type {};

template<typename ...Options>
struct make_set_member_hook
{
  typedef typename pack_options<hook_defaults, Options...>::type                  packed_options;
  typedef generic_hook<RbTreeAlgorithms,
                       rbtree_node_traits<typename packed_options::void_pointer, packed_options::optimize_size>,
                       member_tag, packed_options::link_mode,
                       NoBaseHookId>                                              implementation_defined;
  typedef implementation_defined                                                  type;
};

template<typename ...Options>
 class set_member_hook : public make_set_member_hook<Options...>::type 
{

};

} // namespace intrusive
} // namespace libs
} // namespace intrusive

#endif //AUTODDS_LIBS_INTRUSIVE_SET_HOOK_HPP_