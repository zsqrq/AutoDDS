//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_BS_SET_HOOK_HPP_
#define AUTODDS_LIBS_INTRUSIVE_BS_SET_HOOK_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/tree_node.hpp"
#include "libs/intrusive/options.hpp"
#include "libs/intrusive/detail/generic_hook.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<typename ...Options>
struct make_bs_set_base_hook
{
  typedef typename pack_options<hook_defaults, Options...>::type                      packed_options;
  typedef generic_hook< BsTreeAlgorithms, tree_node_traits<typename packed_options::void_pointer>,
                        typename packed_options::tag,
                        packed_options::link_mode, BsTreeBaseHookId>                  implementation_defined;
  typedef implementation_defined                                                      type;
};

template<typename ...Options>
class bs_set_base_hook : public make_bs_set_base_hook<Options...>::type
{};

template<typename ...Options>
struct make_bs_set_member_hook
{
  typedef typename pack_options< hook_defaults, Options...>::type                     packed_options;
  typedef generic_hook< BsTreeAlgorithms, tree_node_traits<typename packed_options::void_pointer>,
                        member_tag,
                        packed_options::link_mode, NoBaseHookId>                      implementation_defined;
  typedef implementation_defined                                                      type;
};

template<typename ...Options>
class bs_set_member_hook : public make_bs_set_member_hook<Options...>::type
{};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_BS_SET_HOOK_HPP_