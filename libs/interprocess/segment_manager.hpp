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

  //!Allocates nbytes bytes. This function is only used in
  //!single-segment management. Never throws
  void* allocate_aligned(size_type nbytes, size_type alignment, const std::nothrow_t&)
  {
    return MemoryAlgorithm::allocate_aligned(nbytes, alignment);
  }

  //!Allocates nbytes bytes. This function is only used in
  //!single-segment management. Throws bad_alloc when fails
  void* allocate_aligned(size_type nbytes, size_type alignment)
  {
    void * ret = MemoryAlgorithm::allocate_aligned(nbytes, alignment);
    if(!ret)
      throw bad_alloc();
    return ret;
  }

  template<class T>
  T *allocation_command(autodds::libs::interprocess::allocation_type command, size_type limit_size,
                        size_type &prefer_in_recvd_out_size, T *&reuse)
  {
    T *ret = MemoryAlgorithm::allocation_command
        (command | autodds::libs::interprocess::nothrow_allocation, limit_size, prefer_in_recvd_out_size, reuse);
    if(!(command & autodds::libs::interprocess::nothrow_allocation) && !ret)
      throw bad_alloc();
    return ret;
  }

  void *raw_allocation_command(autodds::libs::interprocess::allocation_type command,   size_type limit_objects,
                               size_type &prefer_in_recvd_out_size, void *&reuse, size_type sizeof_object = 1)
  {
    void *ret = MemoryAlgorithm::raw_allocation_command
        ( command | autodds::libs::interprocess::nothrow_allocation, limit_objects,
          prefer_in_recvd_out_size, reuse, sizeof_object);
    if(!(command & autodds::libs::interprocess::nothrow_allocation) && !ret)
      throw bad_alloc();
    return ret;
  }

  //!Deallocates the bytes allocated with allocate/allocate_many()
  //!pointed by addr
  void deallocate(void* addr)
  {
    MemoryAlgorithm::deallocate(addr);
  }

  //!Increases managed memory in extra_size bytes more. This only works
  //!with single-segment management.
  void grow(size_type extra_size)
  {
    MemoryAlgorithm::grow(extra_size);
  }

  //!Decreases managed memory to the minimum. This only works
  //!with single-segment management.
  void shrink_to_fit()
  {
    MemoryAlgorithm::shrink_to_fit();
  }

  //!Returns the result of "all_memory_deallocated()" function
  //!of the used memory algorithm
  bool all_memory_deallocated()
  {
    return MemoryAlgorithm::all_memory_deallocated();
  }

  //!Returns the result of "check_sanity()" function
  //!of the used memory algorithm
  bool check_sanity()
  {
    return MemoryAlgorithm::check_sanity();
  }

  //!Writes to zero free memory (memory not yet allocated)
  //!of the memory algorithm
  void zero_free_memory()
  {
    MemoryAlgorithm::zero_free_memory();
  }

  //!Returns the size of the buffer previously allocated pointed by ptr
  size_type size(const void *ptr) const
  {
    return MemoryAlgorithm::size(ptr);
  }

 protected:
  void* prot_anonymous_construct(size_type num, bool dothrow, ipcdetail::in_place_interface &table)
  {
    typedef ipcdetail::block_header<size_type> block_header_t;
    block_header_t block_info(size_type(table.size*num),
                              size_type(table.alignment), anonymous_type, 1, 0);

    //Allocate memory
    void *ptr_struct = this->allocate(block_info.total_size(), nothrow<>::get());

    //Check if there is enough memory
    if(!ptr_struct){
      if(dothrow){
        throw bad_alloc();
      }
      else{
        return 0;
      }
    }

    //Build scoped ptr to avoid leaks with constructor exception
    ipcdetail::mem_algo_deallocator<MemoryAlgorithm> mem(ptr_struct, *this);

    //Now construct the header
    block_header_t * hdr = ::new(ptr_struct, autodds_container_new_t()) block_header_t(block_info);
    void *ptr = 0; //avoid gcc warning
    ptr = hdr->value();

    //Now call constructors
    ipcdetail::array_construct(ptr, num, table);

    //All constructors successful, we don't want erase memory
    mem.release();
    return ptr;
  }

  //!Calls the destructor and makes an anonymous deallocate
  void prot_anonymous_destroy(const void *object, ipcdetail::in_place_interface &table)
  {

    //Get control data from associated with this object
    typedef ipcdetail::block_header<size_type> block_header_t;
    block_header_t *ctrl_data = block_header_t::block_header_from_value(object, table.size, table.alignment);

    //-------------------------------
    //scoped_lock<rmutex> guard(m_header);
    //-------------------------------

    if(ctrl_data->alloc_type() != anonymous_type){
      //This is not an anonymous object, the pointer is wrong!
      AUTODDS_ASSERT(0);
    }

    //Call destructors and free memory
    //Build scoped ptr to avoid leaks with destructor exception
    std::size_t destroyed = 0;
    table.destroy_n(const_cast<void*>(object), ctrl_data->m_value_bytes/table.size, destroyed);
    this->deallocate(ctrl_data);
  }

};

//!This object is placed in the beginning of memory segment and
//!implements the allocation (named or anonymous) of portions
//!of the segment. This object contains two indexes that
//!maintain an association between a name and a portion of the segment.
//!
//!The first index contains the mappings for normal named objects using the
//!char type specified in the template parameter.
//!
//!The second index contains the association for unique instances. The key will
//!be the const char * returned from type_info.name() function for the unique
//!type to be constructed.
//!
//!segment_manager<CharType, MemoryAlgorithm, IndexType> inherits publicly
//!from segment_manager_base<MemoryAlgorithm> and inherits from it
//!many public functions related to anonymous object and raw memory allocation.
//!See segment_manager_base reference to know about those functions.
template<typename CharType,
    typename MemoryAlgorithm,
    template<typename IndexConfig> typename IndexType>
class segment_manager : public segment_manager_base<MemoryAlgorithm>
{
  segment_manager();
  segment_manager(const segment_manager&);
  segment_manager& operator =(const segment_manager&);
  typedef segment_manager_base<MemoryAlgorithm>  segment_manager_base_t;

 public:
  typedef MemoryAlgorithm                                   memory_algorithm;
  typedef typename segment_manager_base_t::void_pointer     void_pointer;
  typedef typename segment_manager_base_t::size_type        size_type;
  typedef typename segment_manager_base_t::difference_type  difference_type;
  typedef CharType                                          char_type;

  typedef segment_manager_base<MemoryAlgorithm>             segment_manager_base_type;
  static const size_type PayloadPerAllocation = segment_manager_base_t::PayloadPerAllocation;

 private:
  typedef ipcdetail::block_header<size_type>       block_header_t;
  typedef ipcdetail::index_config<CharType, MemoryAlgorithm>  index_config_named;
  typedef ipcdetail::index_config<char, MemoryAlgorithm>      index_config_unique;
  typedef IndexType<index_config_named>                       index_type;
  typedef ipcdetail::bool_<is_intrusive_index<index_type>::value >    is_intrusive_t;
  typedef ipcdetail::bool_<is_node_index<index_type>::value>          is_node_index_t;

 public:
  typedef IndexType<index_config_named>                       named_index_t;
  typedef IndexType<index_config_unique>                      unique_index_t;
  typedef ipcdetail::char_ptr_holder<CharType>                char_ptr_holder_t;
  typedef ipcdetail::segment_manager_iterator_transform
      <typename named_index_t::const_iterator
          ,is_intrusive_index<index_type>::value>             named_transform;
  typedef ipcdetail::segment_manager_iterator_transform
      <typename unique_index_t::const_iterator
          ,is_intrusive_index<index_type>::value>             unique_transform;

};

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_