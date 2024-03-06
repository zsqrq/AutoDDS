//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_INTRUSIVE_FWD_HPP_
#define AUTODDS_LIBS_INTRUSIVE_INTRUSIVE_FWD_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif 
#
#ifndef AUTODDS_CSTDINT_HPP
#  include "libs/common/cstdint.hpp"
#endif
#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

#ifdef AUTODDS_HAS_INTPTR_T
using ::autodds::libs::uintptr_t;
#else
typedef std::size_t uintptr_t;
#endif

////////////////////////////
//     Node algorithms
////////////////////////////

template<typename NodeTraits>
class circular_list_algorithms;

template<typename NodeTraits>
class circular_slist_algorithms;

template<typename NodeTraits>
class linear_slist_algorithms;

template<typename NodeTraits>
class bstree_algorithms;

template<typename NodeTraits>
class rbtree_algorithms;

template<typename NodeTraits>
class avltree_algorithms;

template<typename NodeTraits>
class sgtree_algorithms;

template<typename NodeTraits>
class splaytree_algorithms;

template<typename NodeTraits>
class treap_algorithms;

////////////////////////////
//       Containers
////////////////////////////

// slist
template <typename T, typename... Options>
class slist;

template <typename ...Options>
class slist_base_hook;

template <typename ...Options>
class slist_member_hook;

// list
template <typename T, typename... Options>
class list;

template<typename ...Options>
class list_base_hook;

template<typename ...Options>
class list_member_hook;

// rbtree/set/multiset
template<typename T, typename ...Options>
class rbtree;

template<typename T, typename ...Options>
class set;

template<typename T, typename ...Options>
class multiset;

template<typename ...Options>
class set_base_hook;

template<typename ...Options>
class set_member_hook;

// splaytree / splay_set / splay_multiset
template<typename T, typename ...Options>
class splaytree;

template<typename T, typename ...Options>
class splay_set;

template<typename T, typename ...Options>
class splay_multiset;


// avltree / avl_set / avl_multiset
template<typename T, typename ...Options>
class avltree;

template<typename T, typename ...Options>
class avl_set;

template<typename T, typename ...Options>
class avl_multiset;

template<typename ...Options>
class avl_set_base_hook;

template<typename ...Options>
class avl_set_member_hook;

// treap / treap_set / treap_multiset
template<typename T, typename ...Options>
class treap;

template<typename T, typename ...Options>
class treap_set;

template<typename T, typename ...Options>
class treap_multiset;

// sgtree / sg_set / sg_multiset
template<typename T, typename ...Options>
class sgtree;

template<typename T, typename ...Options>
class sg_set;

template<typename T, typename ...Options>
class sg_multiset;

template<typename T, typename ...Options>
class bstree;

template<typename T, typename ...Options>
class bs_set;

template<typename T, typename ...Options>
class bs_multiset;

template<typename ...Options>
class bs_set_base_hook;

template<typename ...Options>
class bs_set_member_hook;

//hashtable/unordered_set/unordered_multiset
template<typename T, typename ...Options>
class hashtable;

template<typename T, typename ...Options>
class unordered_set;

template<typename T, typename ...Options>
class unordered_multiset;

template<typename ...Options>
class unordered_set_base_hook;

template<typename ...Options>
class unordered_set_member_hook;

template<typename ...Options>
class any_base_hook;

template<typename ...Options>
class any_member_hook;

//Options
template<bool Enabled>
struct constant_time_size;

template<typename SizeType>
struct size_type;

template<typename Compare>
struct compare;

template<bool Enabled>
struct floating_point;

template<typename Equal>
struct equal;

template<typename Priority>
struct priority;

template<typename Hash>
struct hash;

template<typename ValueTraits>
struct value_traits;

template<
    typename Parent,
    typename MemberHook,
    MemberHook Parent::* PtrToMember
    >
struct member_hook;

template<typename Functor>
struct function_hook;

template<typename BaseHook>
struct base_hook;

template<typename VoidPointer>
struct void_pointer;

template<typename Tag>
struct tag;

template<link_mode_type LinkType>
struct link_mode;

template<bool Enabled> struct
optimize_size;

template<bool Enabled>
struct linear;

template<bool Enabled>
struct cache_last;

template<typename BucketTraits>
struct bucket_traits;

template<bool Enabled>
struct store_hash;

template<bool Enabled>
struct optimize_multikey;

template<bool Enabled>
struct power_2_buckets;

template<bool Enabled>
struct cache_begin;

template<bool Enabled>
struct compare_hash;

template<bool Enabled>
struct incremental;

//Value traits
template<typename ValueTraits>
struct value_traits;

template<
    typename Parent,
    typename MemberHook,
    MemberHook Parent::* PtrToMember
    >
struct member_hook;

template< typename Functor>
struct function_hook;

template<typename BaseHook>
struct base_hook;

template<typename T, typename NodeTraits, link_mode_type LinkMode = safe_link>
struct derivation_value_traits;

template<typename NodeTraits, link_mode_type LinkMode = normal_link>
struct trivial_value_traits;

//Additional utilities

template<typename VoidPointer, std::size_t Alignment>
struct max_pointer_plus_bits;

template<std::size_t Alignment>
struct max_pointer_plus_bits<void *, Alignment>;

template<typename Pointer, std::size_t NumBits>
struct pointer_plus_bits;

template<typename T, std::size_t NumBits>
struct pointer_plus_bits<T *, NumBits>;

template<typename Ptr>
struct pointer_traits;

template<typename T>
struct pointer_traits<T *>;

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_INTRUSIVE_FWD_HPP_