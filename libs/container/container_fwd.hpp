//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_CONTAINER_CONTAINER_FWD_HPP_
#define AUTODDS_LIBS_CONTAINER_CONTAINER_FWD_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_STD_FWD_HPP_
#include "libs/container/detail/std_fwd.hpp"
#endif

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {
}}}}

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

namespace ati = autodds::intrusive;
namespace atid = autodds::intrusive::detail;

} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds

namespace autodds {
namespace libs {
namespace container {
namespace pmr {

namespace ati = autodds::intrusive;
namespace atid = autodds::intrusive::detail;

} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds

#include <cstddef>

namespace autodds {
namespace libs {
namespace container {

template<typename T>
class new_allocator;

template<typename T, typename Allocator = void, typename Options = void>
class vector;

template<typename T, typename Allocator = void>
class stable_vector;

template< typename T, std::size_t Capacity, typename Options = void>
class static_vector;

template<typename T, typename Allocator = void, typename Options = void>
class small_vector_bas;

template<typename T, std::size_t N, typename Allocator = void, typename Options = void >
class small_vector;

template<typename T, typename Allocator = void, typename Options = void>
class devector;

template<typename T, typename Allocator = void, typename Options = void>
class deque;

template<typename T, typename Allocator = void>
class list;

template<typename T, typename Allocator = void>
class slist;

template <
    typename Key,
    typename Compare  = std::less<Key>,
    typename Allocator = void,
    typename Options = void>
class set;

template <typename Key
    ,typename Compare  = std::less<Key>
    ,typename Allocator = void
    ,typename Options = void >
class multiset;

template <typename Key
    ,typename T
    ,typename Compare  = std::less<Key>
    ,typename Allocator = void
    ,typename Options = void >
class map;

template <class Key
    ,class T
    ,class Compare  = std::less<Key>
    ,class Allocator = void
    ,class Options = void >
class multimap;

template <class Key
    ,class Compare  = std::less<Key>
    ,class Allocator = void >
class flat_set;

template <class Key
    ,class Compare  = std::less<Key>
    ,class Allocator = void >
class flat_multiset;

template <class Key
    ,class T
    ,class Compare  = std::less<Key>
    ,class Allocator = void >
class flat_map;

template <class Key
    ,class T
    ,class Compare  = std::less<Key>
    ,class Allocator = void >
class flat_multimap;

template < class Key
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
using small_flat_set = flat_set<Key, Compare, small_vector<Key, N, SmallVectorAllocator, SmallVectorOptions>>;

template < class Key
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
using small_flat_multiset = flat_multiset<Key, Compare, small_vector<Key, N, SmallVectorAllocator, SmallVectorOptions>>;

template < class Key
    , class T
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
using small_flat_map = flat_map<Key, T, Compare, small_vector<std::pair<Key, T>, N, SmallVectorAllocator, SmallVectorOptions>>;

template < class Key
    , class T
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
using small_flat_multimap = flat_multimap<Key, T, Compare, small_vector<std::pair<Key, T>, N, SmallVectorAllocator, SmallVectorOptions>>;

template < class Key
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
struct small_flat_set_of
{
  typedef flat_set<Key, Compare, small_vector<Key, N, SmallVectorAllocator, SmallVectorOptions> > type;
};

//! A portable metafunction to obtain a small_flat_multiset
template < class Key
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
struct small_flat_multiset_of
{
  typedef flat_multiset<Key, Compare, small_vector<Key, N, SmallVectorAllocator, SmallVectorOptions> > type;
};

//! A portable metafunction to obtain a small_flat_map
template < class Key
    , class T
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
struct small_flat_map_of
{
  typedef flat_map<Key, T, Compare, small_vector<std::pair<Key, T>, N, SmallVectorAllocator, SmallVectorOptions> > type;
};

//! A portable metafunction to obtain a small_flat_multimap
template < class Key
    , class T
    , std::size_t N
    , class Compare  = std::less<Key>
    , class SmallVectorAllocator = void
    , class SmallVectorOptions   = void  >
struct small_flat_multimap_of
{
  typedef flat_multimap<Key, T, Compare, small_vector<std::pair<Key, T>, N, SmallVectorAllocator, SmallVectorOptions> > type;
};

template <class CharT
    ,class Traits = std::char_traits<CharT>
    ,class Allocator  = void >
class basic_string;

typedef basic_string <char>   string;
typedef basic_string<wchar_t> wstring;

static const std::size_t ADP_nodes_per_block    = 256u;
static const std::size_t ADP_max_free_blocks    = 2u;
static const std::size_t ADP_overhead_percent   = 1u;
static const std::size_t ADP_only_alignment     = 0u;

template < class T
    , std::size_t NodesPerBlock   = ADP_nodes_per_block
    , std::size_t MaxFreeBlocks   = ADP_max_free_blocks
    , std::size_t OverheadPercent = ADP_overhead_percent
    , unsigned Version = 2
>
class adaptive_pool;

template < class T
    , unsigned Version = 2
    , unsigned int AllocationDisableMask = 0>
class allocator;

static const std::size_t NodeAlloc_nodes_per_block = 256u;

template< class T
        , std::size_t NodesPerBlock = NodeAlloc_nodes_per_block
        , std::size_t Version = 2>
class node_allocator;

namespace pmr {

class memory_resource;

template<typename T>
class polymorphic_allocator;

class monotonic_buffer_resource;

struct pool_options;

template <typename Allocator>
class resource_adaptor_imp;

class unsynchronized_pool_resource;

class synchronized_pool_resource;

}  //namespace pmr {

struct ordered_range_t
{};

static const ordered_range_t ordered_range = ordered_range_t();

struct ordered_unique_range_t : public ordered_range_t
{};

static const ordered_unique_range_t ordered_unique_range = ordered_unique_range_t();

struct default_init_t
{};

static const default_init_t default_init = default_init_t();

struct value_init_t
{};

static const value_init_t value_init = value_init_t();

namespace container_detail_really_deep_namespace {

//Otherwise, gcc issues a warning of previously defined
//anonymous_instance and unique_instance
struct dummy
{
  dummy()
  {
    (void)ordered_range;
    (void)ordered_unique_range;
    (void)default_init;
  }
};

}  //detail_really_deep_namespace

typedef const std::piecewise_construct_t & piecewise_construct_t;

} // namespace container
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_CONTAINER_CONTAINER_FWD_HPP_