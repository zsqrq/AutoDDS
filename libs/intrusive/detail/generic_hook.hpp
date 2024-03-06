//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_GENERIC_HOOK_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_GENERIC_HOOK_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/assert.hpp"
#include "libs/intrusive/detail/node_holder.hpp"
#include "libs/intrusive/detail/algo_type.hpp"
#include "libs/common/static_assert.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

namespace detail {

template<link_mode_type link_mode>
struct link_dispatch
{};

template<typename Hook>
AUTODDS_INTRUSIVE_FORCEINLINE void destructor(Hook& hook, link_dispatch<safe_link>)
{
  //If this assertion raises, you might have destroyed an object
  //while it was still inserted in a container that is alive.
  //If so, remove the object from the container before destroying it.
  (void)hook;
  AUTODDS_ASSERT(!hook.is_linked());
}

template<class Hook>
AUTODDS_INTRUSIVE_FORCEINLINE void destructor_impl(Hook &hook, link_dispatch<auto_unlink>)
{  hook.unlink();  }

template<class Hook>
AUTODDS_INTRUSIVE_FORCEINLINE void destructor_impl(Hook &, link_dispatch<normal_link>)
{}

} // namespace detail

enum base_hook_type
{
  NoBaseHookId,
  ListBaseHookId,
  SlistBaseHookId,
  RbTreeBaseHookId,
  HashBaseHookId,
  AvlTreeBaseHookId,
  BsTreeBaseHookId,
  TreapTreeBaseHookId,
  AnyBaseHookId
};

template <typename HookTags, unsigned int>
struct hook_tags_definer{};

template <typename HookTags>
struct hook_tags_definer<HookTags, ListBaseHookId>
{  typedef HookTags default_list_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, SlistBaseHookId>
{  typedef HookTags default_slist_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, RbTreeBaseHookId>
{  typedef HookTags default_rbtree_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, HashBaseHookId>
{  typedef HookTags default_hashtable_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, AvlTreeBaseHookId>
{  typedef HookTags default_avltree_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, BsTreeBaseHookId>
{  typedef HookTags default_bstree_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, TreapTreeBaseHookId>
{  typedef HookTags default_treaptree_hook;  };

template <typename HookTags>
struct hook_tags_definer<HookTags, AnyBaseHookId>
{  typedef HookTags default_any_hook;  };

template<
    typename NodeTraits,
    typename Tag,
    link_mode_type LinkMode,
    base_hook_type BaseHookType
>
struct hooktags_impl
{
  static const link_mode_type link_mode = LinkMode;
  typedef Tag tag;
  typedef NodeTraits node_traits;
  static const bool is_base_hook = !::autodds::libs::move_detail::is_same<Tag, member_tag>::value;
  static const bool safemode_or_autounlink = is_safe_autounlink<link_mode>::value;
  static const unsigned int type = BaseHookType;
};

template<algo_types Algo, typename NodeTraits, typename Tag,
    link_mode_type LinkMode, base_hook_type BaseHookType>
class generic_hook
{

};


} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_GENERIC_HOOK_HPP_