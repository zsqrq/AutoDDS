//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_INTERPROCESS_FWD_HPP_
#define AUTODDS_LIBS_INTERPROCESS_INTERPROCESS_FWD_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#ifndef AUTODDS_LIBS_COMMON_CSTDINT_HPP_
#  include "libs/common/cstdint.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/std_fwd.hpp"
#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include <cstddef>

// Standard predeclarations
namespace autodds { namespace libs { namespace intrusive { } } }
namespace autodds { namespace libs { namespace interprocess
{ namespace bi = autodds::libs::intrusive; } } }

namespace autodds {
namespace libs {
namespace interprocess {
class permissions;
class shared_memory_object;

#if defined(AUTODDS_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)
class xsi_shared_memory;
#endif

class file_mapping;
class mapped_region;

// Mutexes
class null_mutex;
class interprocess_mutex;
class interprocess_recursive_mutex;
class named_mutex;
class named_recursive_mutex;
class interprocess_semaphore;
class named_semaphore;

// Mutex families
struct mutex_family;
struct null_mutex_family;

// Other synchronization classes
class interprocess_sharable_mutex;
class interprocess_condition;

// Locks
template<typename Mutex>
class scoped_lock;

template <typename SharableMutex>
class sharable_lock;

template <typename UpgradableMutex>
class upgradable_lock;

// STL compatible allocators
template<typename T, typename SegmentManager>
class allocator;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64>
class node_allocator;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64>
class private_node_allocator;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64>
class cached_node_allocator;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64
    , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class adaptive_pool;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64
    , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class private_adaptive_pool;

template<typename T, typename SegmentManager, std::size_t NodesPerBlock = 64
    , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class cached_adaptive_pool;

// offset_ptr
static constexpr std::size_t offset_type_alignment = 0;

#  ifdef AUTODDS_HAS_INTPTR_T
using ::autodds::libs::uintptr_t;
#  else
typedef std::size_t uintptr_t;
#  endif

template <typename T, typename DifferenceType = std::ptrdiff_t
    , typename OffsetType = uintptr_t, std::size_t Alignment = offset_type_alignment>
class offset_ptr;

//  Memory allocation algorithms
template<typename MutexFamily, typename VoidMutex = offset_ptr<void> >
class simple_seq_fit;

template<typename MutexFamily, typename VoidMutex = offset_ptr<void>, std::size_t MemAligment = 0>
class rbtree_best_fit;

// Index Types
template<class IndexConfig> class flat_map_index;
template<class IndexConfig> class iset_index;
template<class IndexConfig> class iunordered_set_index;
template<class IndexConfig> class map_index;
template<class IndexConfig> class null_index;
template<class IndexConfig> class unordered_map_index;

// Segment manager
template<
    typename CharType,
    typename MemoryAlgorithm,
    template<typename IndexConfig> class IndexType>
class segment_manager;

// External buffer managed memory classes
template <
    typename CharType,
    typename MemoryAlgorithm,
    template<typename IndexConfig> class IndexType>
class basic_managed_external_buffer;

typedef basic_managed_external_buffer
    <char,
     rbtree_best_fit<null_mutex_family>,
     iset_index>
    managed_external_buffer;

typedef basic_managed_external_buffer
    <wchar_t
        ,rbtree_best_fit<null_mutex_family>
        ,iset_index>
    wmanaged_external_buffer;

// managed memory classes
template <
    typename CharType,
    typename MemoryAlgorithm,
    template<typename IndexConfig> class IndexType>
class basic_managed_shared_memory;

typedef basic_managed_shared_memory
    <char,
    rbtree_best_fit<mutex_family>,
    iset_index> managed_shared_memory;

typedef basic_managed_shared_memory<
    wchar_t,
    rbtree_best_fit<mutex_family>,
    iset_index> wmanaged_shared_memory;

#if defined(AUTODDS_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)
template <typename CharType,typename MemoryAlgorithm,
template<typename IndexConfig> class IndexType>
class basic_managed_xsi_shared_memory;

typedef basic_managed_xsi_shared_memory
    <char
        ,rbtree_best_fit<mutex_family>
        ,iset_index>
    managed_xsi_shared_memory;

typedef basic_managed_xsi_shared_memory
    <wchar_t
        ,rbtree_best_fit<mutex_family>
        ,iset_index>
    wmanaged_xsi_shared_memory;
#endif

// Fixed address shared memory
typedef basic_managed_shared_memory
<char,rbtree_best_fit<mutex_family, void*>,iset_index>
fixed_managed_shared_memory;

typedef basic_managed_shared_memory
<wchar_t,rbtree_best_fit<mutex_family, void*>,iset_index>
wfixed_managed_shared_memory;

// Heap memory managed memory classes
template
    <typename CharType
        ,typename MemoryAlgorithm
        ,template<typename IndexConfig> class IndexType>
class basic_managed_heap_memory;

typedef basic_managed_heap_memory
    <char
        ,rbtree_best_fit<null_mutex_family>
        ,iset_index>
    managed_heap_memory;

typedef basic_managed_heap_memory
    <wchar_t
        ,rbtree_best_fit<null_mutex_family>
        ,iset_index>
    wmanaged_heap_memory;

// Mapped file managed memory classes
template<
    typename CharType,
    typename MemoryAlgorithm,
    template<typename IndexConfig> class IndexType>
class basic_managed_mapped_file;

typedef basic_managed_mapped_file
<char,rbtree_best_fit<mutex_family>,iset_index>
managed_mapped_file;

typedef basic_managed_mapped_file
<wchar_t,rbtree_best_fit<mutex_family>,iset_index>
wmanaged_mapped_file;

// Exceptions
class interprocess_exception;
class lock_exception;
class bad_alloc;

// Bufferstream
template <
    typename CharT,
    typename CharTraits = std::char_traits<CharT> >
class basic_bufferbuf;

template <
    typename CharT,
    typename CharTraits = std::char_traits<CharT> >
class basic_ibufferstream;

template <
    typename CharT,
    typename CharTraits = std::char_traits<CharT> >
class basic_obufferstream;

template <
    typename CharT,
    typename CharTraits = std::char_traits<CharT> >
class basic_bufferstream;

// Vectorstream
template <
    typename CharVector,
    typename CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_vectorbuf;

template <
    typename CharVector,
    typename CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_ivectorstream;

template <
    typename CharVector,
    typename CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_ovectorstream;

template <
    typename CharVector,
    typename CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_vectorstream;

// Smart pointers
template<class T, class Deleter>
class scoped_ptr;

template<class T, class VoidPointer>
class intrusive_ptr;

template<class T, class VoidAllocator, class Deleter>
class shared_ptr;

template<class T, class VoidAllocator, class Deleter>
class weak_ptr;

// IPC
template<class VoidPointer>
class message_queue_t;

typedef message_queue_t<offset_ptr<void> > message_queue;

}
}
}

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_INTERPROCESS_FWD_HPP_