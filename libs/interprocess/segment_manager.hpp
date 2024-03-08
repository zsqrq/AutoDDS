//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/core/no_exceptions_support.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/transform_iterator.hpp"

#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/detail/nothrow.hpp"
#include "libs/interprocess/detail/segment_manager_helper.hpp"
#include "libs/interprocess/detail/named_proxy.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/offset_ptr.hpp"
#include "libs/interprocess/indexes/iset_index.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/allocators/allocator.hpp"
#include "libs/interprocess/smart_ptr/deleter.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/sync/scoped_lock.hpp"

#include "libs/container/detail/minimal_char_traits_header.hpp"
#include "libs/container/detail/placement_new.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"
#include "libs/common/assert.hpp"

#include <cstddef>
#include <exception>

namespace autodds {
namespace libs {
namespace interprocess {

//!This object is the public base class of segment manager.
//!This class only depends on the memory allocation algorithm
//!and implements all the allocation features not related
//!to named or unique objects.
template<typename MemoryAlgorithm>
class segment_manager_base : private MemoryAlgorithm
{
 public:
  typedef segment_manager_base<MemoryAlgorithm>     segment_manager_base_type;
  typedef typename MemoryAlgorithm::void_pointer    void_pointer;
  typedef typename MemoryAlgorithm::mutex_family    mutex_family;
  typedef MemoryAlgorithm                           memory_algorithm;

  typedef typename MemoryAlgorithm::multiallocation_chain   multiallocation_chain;
  typedef typename MemoryAlgorithm::difference_type         difference_type;
  typedef typename MemoryAlgorithm::size_type        size_type;

  //!This constant indicates the payload size
  //!associated with each allocation of the memory algorithm
  static const size_type PayloadAllocation = MemoryAlgorithm::PayloadAllocation;

  //!Constructor of the segment_manager_base
  segment_manager_base(size_type sz, size_type reserved_bytes)
  : MemoryAlgorithm(sz, reserved_bytes)
  {
    AUTODDS_ASSERT(sizeof(segment_manager_base<MemoryAlgorithm>) == sizeof (MemoryAlgorithm));
  }

  size_type get_size() const
  {
    return MemoryAlgorithm::get_size();
  }

  size_type get_free_memory() const
  {
    return MemoryAlgorithm::get_free_memory();
  }

  static size_type get_min_size(size_type size)
  {
    return MemoryAlgorithm::get_min_size(size);
  }

  void* allocate(size_type nbytes, const std::nothrow_t&)
  {
    return MemoryAlgorithm::allocate(nbytes);
  }

  MemoryAlgorithm& get_momory_algorithm()
  {
    return static_cast<MemoryAlgorithm&>(*this);
  }

  void allocate_many(size_type elem_type, size_type n_element, multiallocation_chain& chain)
  {
    size_type prev_size = chain.size();
    MemoryAlgorithm::allocate_many(elem_type, n_element, chain);

    if (!elem_type || chain.size() == prev_size)
    {
      throw bad_alloc();
    }
  }

  void allocate_many(const size_type* element_length, size_type n_elems, size_type elem_size,
                     multiallocation_chain& chain)
  {
    size_type prev_size = chain.size();
    MemoryAlgorithm::allocate_many(element_length, n_elems, elem_size, chain);
    if (!elem_size || chain.size() == prev_size)
    {
      throw bad_alloc();
    }
  }

  void allocate_many(const std::nothrow_t &, size_type elem_bytes, size_type n_elements, multiallocation_chain &chain)
  {  MemoryAlgorithm::allocate_many(elem_bytes, n_elements, chain); }

  void allocate_many(std::nothrow_t& , const size_type* elem_sizes, size_type n_elem,
                     size_type sizeof_elem, multiallocation_chain& chain)
  {
    MemoryAlgorithm::allocate_many(elem_sizes, n_elem, sizeof_elem, chain);
  }

  void deallocate_many(multiallocation_chain& chain)
  {
    MemoryAlgorithm::deallocate_many(chain);
  }

  void * allocate(size_type nbytes)
  {
    void * ret = MemoryAlgorithm::allocate(nbytes);
    if(!ret)
      throw bad_alloc();
    return ret;
  }

  void* allocate_aligned(size_type nbytes, size_type alignment, const std::nothrow_t&)
  {
    return MemoryAlgorithm::allocate_aligned(nbytes, alignment);
  }

  void* allocate_aligned(size_type nbytes, size_type alignment)
  {
    void * ret = MemoryAlgorithm::allocate_aligned(nbytes, alignment);
    if(!ret)
      throw bad_alloc();
    return ret;
  }

};

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_