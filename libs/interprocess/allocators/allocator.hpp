//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_
#define AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/containers/allocation_type.hpp"
#include "libs/container/detail/multiallocation_chain.hpp"
#include "libs/interprocess/allocators/detail/allocator_common.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/containers/version_type.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/common/assert.hpp"
#include "libs/core/addressof.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/container/detail/placement_new.hpp"

#include <cstddef>
#include <stdexcept>

namespace autodds {
namespace libs {
namespace interprocess {

//!An STL compatible allocator that uses a segment manager as
//!memory source. The internal pointer type will of the same type (raw, smart) as
//!"typename SegmentManager::void_pointer" type. This allows
//!placing the allocator in shared memory, memory mapped-files, etc...
template <typename T, typename SegmentManager>
class allocator
{
 public:
  //Segment manager
  typedef SegmentManager                                segment_manager;
  typedef typename SegmentManager::void_pointer         void_pointer;

 private:
  //Self type
  typedef allocator<T, SegmentManager>   self_t;

  //Pointer to void
  typedef typename segment_manager::void_pointer  aux_pointer_t;

  typedef typename autodds::libs::intrusive::pointer_traits
      <aux_pointer_t>::template rebind_pointer<const void>::type       cvoid_ptr;
  typedef typename autodds::libs::intrusive::pointer_traits
      <cvoid_ptr>::template rebind_pointer<segment_manager>::type      alloc_ptr_t;

  //Not assignable from related allocator
  template<class T2, class SegmentManager2>
  allocator& operator =(const allocator<T2, SegmentManager2>&);

  //Not assignable from other allocator
  allocator& operator=(const allocator&);

  //Pointer to the segment_manager
  alloc_ptr_t mp_mngr;

 public:
  typedef T    value_type;
  typedef typename autodds::libs::intrusive::pointer_traits
      <cvoid_ptr>::template rebind_pointer<T>::type                   pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <pointer>::template rebind_pointer<const T>::type               const_pointer;
  typedef typename ipcdetail::add_reference<value_type>::type         reference;
  typedef typename ipcdetail::add_reference<const value_type>::type   const_reference;
  typedef typename segment_manager::size_type                         size_type;
  typedef typename segment_manager::difference_type                   difference_type;

  typedef autodds::libs::interprocess::version_type<allocator, 2>     version;
  typedef autodds::libs::container::dtl::transform_multiallocation_chain
      <typename SegmentManager::multiallocation_chain, T>             multiallocation_chain;

  template<class T2>
  struct rebind
  {
    typedef allocator<T2, SegmentManager>  other;
  };

  segment_manager* get_segment_manager() const {
    return ipcdetail::to_raw_pointer(mp_mngr);
  }

  //!Constructor from the segment manager.
  allocator(segment_manager* segment_mgr)
      : mp_mngr(segment_mgr)
  {}

  allocator(const allocator& other)
      : mp_mngr(other.get_segment_manager())
  {}

  template<class T2>
  allocator(const allocator<T2, SegmentManager> &other)
      : mp_mngr(other.get_segment_manager())
  {}

  //!Allocates memory for an array of count elements.
  pointer allocate(size_type count, cvoid_ptr hint = 0)
  {
    (void)hint;
    if (size_overflows<sizeof(T)>(count)) {
      throw bad_alloc();
    }
    return pointer(static_cast<value_type*>(mp_mngr->allocate(count * sizeof(T))));
  }

  //!Deallocates memory previously allocated.
  void deallocate(const pointer& ptr, size_type)
  {
    mp_mngr->deallocate((void*)ipcdetail::to_raw_pointer(ptr));
  }

  //!Returns the number of elements that could be allocated.
  size_type max_size() const
  {
    return mp_mngr->get_size() / sizeof(T);
  }

  //!Swap segment manager. Does not throw. If each allocator is placed in
  //!different memory segments, the result is undefined.
  friend void swap(self_t& allocater1,self_t& allocater2 )
  {
    autodds::libs::adl_move_swap(allocater1.mp_mngr, allocater2.mp_mngr);
  }

  //!Returns maximum the number of objects the previously allocated memory
  //!pointed by p can hold. This size only works for memory allocated with
  //!allocate, allocation_command and allocate_many.
  size_type size(const pointer &p) const
  {
    return (size_type)mp_mngr->size(ipcdetail::to_raw_pointer(p))/sizeof(T);
  }

  pointer allocation_command(autodds::libs::interprocess::allocation_type command,
                             size_type limit_size, size_type &prefer_in_recvd_out_size, pointer &reuse)
  {
    value_type *reuse_raw = ipcdetail::to_raw_pointer(reuse);
    pointer const p = mp_mngr->allocation_command(command, limit_size, prefer_in_recvd_out_size, reuse_raw);
    reuse = reuse_raw;
    return p;
  }

  //!Allocates many elements of size elem_size in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. The elements must be deallocated
  //!with deallocate(...)
  void allocate_many(size_type elem_size, size_type num_elements, multiallocation_chain &chain)
  {
    if(size_overflows<sizeof(T)>(elem_size)){
      throw bad_alloc();
    }
    mp_mngr->allocate_many(elem_size*sizeof(T), num_elements, chain);
  }

  //!Allocates n_elements elements, each one of size elem_sizes[i]in a
  //!contiguous block
  //!of memory. The elements must be deallocated
  void allocate_many(const size_type *elem_sizes, size_type n_elements, multiallocation_chain &chain)
  {
    mp_mngr->allocate_many(elem_sizes, n_elements, sizeof(T), chain);
  }

  //!Allocates many elements of size elem_size in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. The elements must be deallocated
  //!with deallocate(...)
  void deallocate_many(multiallocation_chain &chain)
  {
    mp_mngr->deallocate_many(chain);
  }

  //!Allocates just one object. Memory allocated with this function
  //!must be deallocated only with deallocate_one().
  pointer allocate_one()
  {
    return this->allocate(1);
  }

  //!Allocates many elements of size == 1 in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. Memory allocated with this function
  //!must be deallocated only with deallocate_one().
  void allocate_individual(size_type num_elements, multiallocation_chain &chain)
  {
    this->allocate_many(1, num_elements, chain);
  }

  //!Deallocates memory previously allocated with allocate_one().
  //!You should never use deallocate_one to deallocate memory allocated
  //!with other functions different from allocate_one(). Never throws
  void deallocate_one(const pointer &p)
  {
    return this->deallocate(p, 1);
  }

  //!Returns address of mutable object.
  pointer address(reference value) const
  {
    return pointer(autodds::libs::addressof(value));
  }

  //!Returns address of non mutable object.
  const_pointer address(const_reference value) const
  {
    return const_pointet(autodds::libs::addressof(value));
  }

  template<class P>
  void construct(const pointer &ptr, AUTODDS_FWD_REF(P) p)
  {
    ::new((void*)ipcdetail::to_raw_pointer(ptr), autodds_container_new_t()) value_type(::autodds::libs::forward<P>(p));
  }

  void destroy(const pointer &ptr)
  {
    AUTODDS_ASSERT(ptr != 0);
    (*ptr).~value_type();
  }

};

template<typename T, typename SegmentManager>
inline bool operator==(const allocator<T , SegmentManager>  &alloc1,
                       const allocator<T, SegmentManager>  &alloc2)
{
  return alloc1.get_segment_manager() == alloc2.get_segment_manager();
}

template<typename T, typename SegmentManager>
inline bool operator!=(const allocator<T, SegmentManager>  &alloc1,
                       const allocator<T, SegmentManager>  &alloc2)
{
  return alloc1.get_segment_manager() != alloc2.get_segment_manager();
}

} // namespace interprocess

template<typename T>
struct has_trivial_destructor;

template<typename T, typename SegmentManager>
struct has_trivial_destructor<autodds::libs::interprocess::allocator<T, SegmentManager>>
{
  static const bool value = true;
};

} // namespace libs
} // namespace autodds

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_ALLOCATOR_HPP_