//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_
#define AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_

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
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/core/addressof.hpp"
#include "libs/common/assert.hpp"

#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/sync/scoped_lock.hpp"
#include "libs/interprocess/containers/allocation_type.hpp"
#include "libs/container/detail/multiallocation_chain.hpp"
#include "libs/interprocess/mem_algo/detail/mem_algo_common.hpp"
#include "libs/interprocess/detail/segment_manager_helper.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/container/detail/placement_new.hpp"
#include "libs/move/adl_move_swap.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

template <typename T>
struct sizeof_value
{
  static const std::size_t value = sizeof(T);
};

template<>
struct sizeof_value<void>
{
  static const std::size_t value = sizeof(void*);
};

template <>
struct sizeof_value<const void>
{
  static const std::size_t value = sizeof(void*);
};

template <>
struct sizeof_value<volatile void>
{
  static const std::size_t value = sizeof(void*);
};

template <>
struct sizeof_value<const volatile void>
{
  static const std::size_t value = sizeof(void*);
};

namespace ipcdetail {

//!Object function that creates the node allocator if it is not created and
//!increments reference count if it is already created
template<typename NodePool>
struct get_or_create_node_pool_func
{
  void operator()()
  {
    mp_node_pool = mp_segment_manager->template find_or_construct
        <NodePool>(autodds::libs::interprocess::unique_instance)(mp_segment_manager);

    if (mp_node_pool != 0)
    {
      mp_node_pool->inc_ref_count();
    }
  }

  get_or_create_node_pool_func(typename NodePool::segment_manager* manager)
      : mp_segment_manager(manager)
  {}

  NodePool                            *mp_node_pool;
  typename NodePool::segment_manager  *mp_segment_manager;
};

template<class NodePool>
inline NodePool *get_or_create_node_pool(typename NodePool::segment_manager *mgnr)
{
  get_or_create_node_pool_func<NodePool> func(mgnr);
  mgnr->atomic_func(func);
  return func.mp_node_pool;
}

//!Object function that decrements the reference count. If the count
//!reaches to zero destroys the node allocator from memory.
//!Never throws
template<typename NodePool>
struct destroy_if_last_link_func
{
  void operator()()
  {
    if (mp_node_pool->dec_ref_count() != 0)
      return;
    mp_node_pool->get_segment_manager()->template destroy<NodePool>(autodds::libs::interprocess::unique_instance);
  }
  destroy_if_last_link_func(NodePool* pool)
      : mp_node_pool(pool)
  {}
  NodePool* mp_node_pool;
};

//!Destruction function, initializes and executes destruction function
//!object. Never throws
template<typename NodePool>
inline void destroy_node_pool_if_last_link(NodePool *pool)
{
  //Get segment manager
  typename NodePool::segment_manager *mngr = pool->get_segment_manager();
  //Execute destruction functor atomically
  destroy_if_last_link_func<NodePool> func(pool);
  mngr->atomic_func(func);
}

template<typename NodePool>
class cache_impl
{
  typedef typename NodePool::segment_manager::void_pointer     void_pointer;

  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<NodePool>::type      node_pool_ptr;
  typedef typename NodePool::multiallocation_chain             multiallocation_chain;
  typedef typename NodePool::segment_manager::size_type        size_type;

  node_pool_ptr             mp_node_pool;
  multiallocation_chain     m_cached_nodes;
  size_type                 m_max_cached_nodes;

 public:
  typedef typename NodePool::segment_manager                   segment_manager;

  cache_impl(segment_manager* segment_mgr, size_type max_cached_nodes)
      : mp_node_pool(get_or_create_node_pool<NodePool>(segment_mgr)),
        m_max_cached_nodes(max_cached_nodes)
  {}

  cache_impl(const cache_impl &other)
      : mp_node_pool(other.get_node_pool()),
        m_max_cached_nodes(other.get_max_cached_nodes())
  {
    mp_node_pool->inc_ref_count();
  }

  ~cache_impl()
  {
    this->deallocate_all_cached_nodes();
    destroy_node_pool_if_last_link(ipcdetail::to_raw_pointer(mp_node_pool));
  }

  NodePool* get_node_pool() const
  {
    return to_raw_pointer(mp_node_pool);
  }

  segment_manager* get_segment_manager() const
  {
    return mp_node_pool->get_segment_maneger();
  }

  size_type get_max_cached_nodes() const
  {
    return m_max_cached_nodes;
  }

  void* cached_allocation()
  {
    // If don't have any cached node, we have to get a new list of free nodes from the pool
    if(m_cached_nodes.empty()) {
      mp_node_pool->allocate_nodes(m_max_cached_nodes/2, m_cached_nodes);
    }
    void *ret = ipcdetail::to_raw_pointer(m_cached_nodes.pop_front());
    return ret;
  }

  void cached_allocation(size_type n, multiallocation_chain &chain)
  {
    size_type count = n, allocated(0);
    AUTODDS_TRY{
        //If don't have any cached node, we have to get a new list of free nodes from the pool
        while(!m_cached_nodes.empty() && count--){
          void *ret = ipcdetail::to_raw_pointer(m_cached_nodes.pop_front());
          chain.push_back(ret);
          ++allocated;
        }

        if(allocated != n){
          mp_node_pool->allocate_nodes(n - allocated, chain);
        }
      }
      AUTODDS_CATCH(...){
        this->cached_deallocation(chain);
        AUTODDS_RETHROW
      } AUTODDS_CATCH_END
  }

  void cached_deallocation(void *ptr)
  {
    if (m_cached_nodes.size() >= m_max_cached_nodes) {
      this->priv_deallocate_n_nodes(m_cached_nodes.size() - m_max_cached_nodes/2);
    }
    m_cached_nodes.push_front(ptr);
  }

  void cached_deallocation(multiallocation_chain& chain)
  {
    m_cached_nodes.splice_after(m_cached_nodes.before_begin(), chain);

    //Check if cache is full
    if(m_cached_nodes.size() >= m_max_cached_nodes){
      this->priv_deallocate_n_nodes(m_cached_nodes.size() - m_max_cached_nodes/2);
    }
  }

  //!Sets the new max cached nodes value. This can provoke deallocations
  //!if "newmax" is less than current cached nodes. Never throws
  void set_max_cached_nodes(size_type newmax)
  {
    m_max_cached_nodes = newmax;
    this->priv_deallocate_remaining_nodes();
  }

  //!Frees all cached nodes.
  //!Never throws
  void deallocate_all_cached_nodes()
  {
    if(m_cached_nodes.empty()) return;
    mp_node_pool->deallocate_nodes(m_cached_nodes);
  }

 private:

  //!Frees all cached nodes at once.
  //!Never throws
  void priv_deallocate_remaining_nodes()
  {
    if(m_cached_nodes.size() > m_max_cached_nodes){
      priv_deallocate_n_nodes(m_cached_nodes.size()-m_max_cached_nodes);
    }
  }

  //!Frees n cached nodes at once. Never throws
  void priv_deallocate_n_nodes(size_type n)
  {
    size_type count(n);
    typename multiallocation_chain::iterator it(m_cached_nodes.before_begin());
    while (count--){
      ++it;
    }
    multiallocation_chain chain;
    chain.splice_after(chain.before_begin(), m_cached_nodes, m_cached_nodes.before_begin(), it, n);
    mp_node_pool->deallocate_nodes(chain);
  }

 public:
  void swap(cache_impl &other)
  {
    ::autodds::libs::adl_move_swap(mp_node_pool, other.mp_node_pool);
    ::autodds::libs::adl_move_swap(m_cached_nodes, other.m_cached_nodes);
    ::autodds::libs::adl_move_swap(m_max_cached_nodes, other.m_max_cached_nodes);
  }

};

template<typename Derived, typename T, typename SegmentManager>
class array_allocation_impl
{
  const Derived* derived() const
  {
    return static_cast<const Derived*>(this);
  }

  Derived* derived()
  {
    return static_cast<Derived*>(this);
  }

  typedef typename SegmentManager::void_pointer   void_pointer;

 public:
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<T>::type                 pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<const T>::type           const_pointer;
  typedef T                                                            value_type;
  typedef typename add_reference<value_type>::type                     reference;
  typedef typename add_reference<const value_type>::type               const_reference;
  typedef typename SegmentManager::size_type                           size_type;
  typedef typename SegmentManager::difference_type                     difference_type;
  typedef autodds::libs::container::dtl::transform_multiallocation_chain
      <typename SegmentManager::multiallocation_chain, T>              multiallocation_chain;

 public:

  //!Returns maximum the number of objects the previously allocated memory
  //!pointed by p can hold. This size only works for memory allocated with
  //!allocate, allocation_command and allocate_many.
  size_type size(const pointer& ptr) const
  {
    return (size_type)this->derived()->get_segment_manager()->size(to_raw_pointer(ptr)) / sizeof(T);
  }

  pointer allocation_command(autodds::libs::interprocess::allocation_type command,
                             size_type limit_size, size_type& prefer_in_recvd_out_size,
                             pointer& reuse)
  {
    value_type* reuse_raw = to_raw_pointer(reuse);
    pointer const p = this->derived()->get_segment_manager()->allocation_command
        (command, limit_size, prefer_in_recvd_out_size, reuse_raw);
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
    this->derived()->get_segment_manager()->allocate_many(elem_size*sizeof(T), num_elements, chain);
  }

  //!Allocates n_elements elements, each one of size elem_sizes[i]in a
  //!contiguous block
  //!of memory. The elements must be deallocated
  void allocate_many(const size_type *elem_sizes, size_type n_elements, multiallocation_chain &chain)
  {
    this->derived()->get_segment_manager()->allocate_many(elem_sizes, n_elements, sizeof(T), chain);
  }

  //!Allocates many elements of size elem_size in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. The elements must be deallocated
  //!with deallocate(...)
  void deallocate_many(multiallocation_chain& chain)
  {
    this->derived()->get_segment_manager()->deallocate_many(chain);
  }

  //!Returns the number of elements that could be
  //!allocated. Never throws
  size_type max_size() const
  {
    return this->derived()->get_segment_manager()->get_size() / sizeof(T);
  }

  //!Returns address of mutable object.
  //!Never throws
  pointer address(reference value) const
  {
    return pointer(autodds::libs::addressof(value));
  }

  //!Returns address of non mutable object.
  //!Never throws
  const_pointer address(const_reference value) const
  {
    return const_pointer(autodds::libs::addressof(value));
  }

  //!Constructs an object
  //!Throws if T's constructor throws
  //!For backwards compatibility with libraries using C++03 allocators
  template<class P>
  void construct(const pointer &ptr, AUTODDS_FWD_REF(P) p)
  {
    ::new((void*)to_raw_pointer(ptr), autodds_container_new_t()) value_type(::autodds::libs::forward<P>(p));
  }

  void destroy(const pointer& ptr)
  {
    AUTODDS_ASSERT(ptr != 0);
    (*ptr).~value_type();
  }
};

template<class Derived, unsigned int Version, class T, class SegmentManager>
class node_pool_allocation_impl : public array_allocation_impl<Derived, T, SegmentManager>
{
  const Derived* derived() const
  {
    return static_cast<const Derived*>(this);
  }

  Derived* derived()
  {
    return static_cast<Derived*>(this);
  }

  typedef typename SegmentManager::void_pointer                     void_pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<const void>::type     cvoid_pointer;

 public:
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<T>::type              pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<const T>::type        const_pointer;
  typedef T                                                         value_type;
  typedef typename add_reference<value_type>::type                  reference;
  typedef typename add_reference<const value_type>::type            const_reference;
  typedef typename SegmentManager::size_type                        size_type;
  typedef typename SegmentManager::difference_type                  difference_type;
  typedef typename autodds::libs::container::dtl::
  transform_multiallocation_chain<
      typename SegmentManager::multiallocation_chain, T>            multiallocation_chain;

  template<int Dummy>
  struct node_pool
  {
    typedef typename Derived::template node_pool<0>::type    type;
    static type* get(void* p)
    {
      return static_cast<type*>(p);
    }
  };

 public:
  //!Allocate memory for an array of count elements.
  pointer allocate(size_type count, cvoid_pointer hint = 0)
  {
    (void)hint;
    typedef typename node_pool<0>::type node_pool_t;
    node_pool_t *pool = node_pool<0>::get(this->derived()->get_node_pool());

    if (size_overflows<sizeof(T)>(count)) {
      throw bad_alloc();
    }
    else if (Version == 1 && count == 1) {
      return pointer(static_cast<value_type*>(pool->allocate_node()));
    }
    else {
      return pointer(static_cast<value_type*>(
                         pool->get_segment_manager()->allocate(count * sizeof(T))));
    }

  }

  //!Deallocate allocated memory. Never throws
  void deallocate(const pointer& ptr, size_type count) AUTODDS_NOEXCEPT
  {
    (void)count;
    typedef typename node_pool<0>::type node_pool_t;
    node_pool_t* pool = node_pool<0>::get(this->derived()->get_node_pool());
    if (Version == 1 && count == 1) {
      pool->deallocate_node(to_raw_pointer(ptr));
    } else {
      pool->get_segment_manager()->deallocate((void*) to_raw_pointer(ptr));
    }
  }

  //!Allocates just one object. Memory allocated with this function
  //!must be deallocated only with deallocate_one().
  pointer allocate_one()
  {
    typedef typename node_pool<0>::type node_pool_t;
    node_pool_t* pool = node_pool<0>::get(this->derived()->get_node_pool());
    return pointer(static_cast<value_type*>(pool->allocate_node()));
  }

  //!Allocates many elements of size == 1 in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. Memory allocated with this function
  //!must be deallocated only with deallocate_one().
  void allocate_individual(size_type num_elements, multiallocation_chain& chain)
  {
    typedef typename node_pool<0>::type node_pool_t;
    node_pool_t* pool = node_pool<0>::get(this->derived()->get_node_pool());
    pool->allocate_nodes(num_elements, chain);
  }

  //!Deallocates memory previously allocated with allocate_one().
  //!You should never use deallocate_one to deallocate memory allocated
  //!with other functions different from allocate_one(). Never throws
  void deallocate_one(const pointer &ptr)
  {
    typedef typename node_pool<0>::type node_pool_t;
    node_pool_t *pool = node_pool<0>::get(this->derived()->get_node_pool());
    pool->deallocate_node(to_raw_pointer(ptr));
  }

  //!Allocates many elements of size == 1 in a contiguous block
  //!of memory. The minimum number to be allocated is min_elements,
  //!the preferred and maximum number is
  //!preferred_elements. The number of actually allocated elements is
  //!will be assigned to received_size. Memory allocated with this function
  //!must be deallocated only with deallocate_one().
  void deallocate_individual(multiallocation_chain& chain)
  {
    node_pool<0>::get(this->derived()->get_node_pool())->deallocate_nodes(chain);
  }

  //!Deallocates all free blocks of the pool
  void deallocate_free_blocks()
  {
    node_pool<0>::get(this->derived()->get_node_pool())->deallocate_free_blocks();
  }

  void deallocate_free_chunks()
  {  node_pool<0>::get(this->derived()->get_node_pool())->deallocate_free_blocks();  }

};

template<class T, class NodePool, unsigned int Version>
class cached_allocator_impl : public array_allocation_impl
    <cached_allocator_impl<T, NodePool, Version>, T, typename NodePool::segment_manager>
{
  cached_allocator_impl& operator =(const cached_allocator_impl& other);
  typedef array_allocation_impl<
      cached_allocator_impl<T, NodePool, Version>,
      T, typename NodePool::segment_manager>                  base_t;

 public:
  typedef NodePool                                                node_pool_t;
  typedef typename NodePool::segment_manager                      segment_manager;
  typedef typename segment_manager::void_pointer                  void_pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>::template rebind_pointer<const void>::type   cvoid_pointer;
  typedef typename base_t::pointer                                pointer;
  typedef typename base_t::size_type                              size_type;
  typedef typename base_t::multiallocation_chain                  multiallocation_chain;
  typedef typename base_t::value_type                             value_type;

 public:
  static const std::size_t DEFAULT_MAX_CACHED_NODES = 64;

  cached_allocator_impl(segment_manager* segment_mgr, size_type max_cached_nodes)
      : m_cache(segment_mgr, max_cached_nodes)
  {}

  cached_allocator_impl(const cached_allocator_impl &other)
      : m_cache(other.m_cache)
  {}

  //!Copy constructor from related cached_adaptive_pool_base. If not present, constructs
  //!a node pool. Increments the reference count of the associated node pool.
  template<class T2, class NodePool2>
  cached_allocator_impl(const cached_allocator_impl<T2, NodePool2, Version>& other)
      : m_cache(other.get_segment_manager(), other.get_max_cached_nodes())
  {}

  //!Returns a pointer to the node pool.
  //!Never throws
  node_pool_t* get_node_pool() const AUTODDS_NOEXCEPT
  {
    return m_cache.get_node_pool();
  }

 private:
  cache_impl<node_pool_t> m_cache;

};

} // namespace ipcdetail


} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_ALLOCATORS_DETAIL_ALLOCATOR_COMMON_HPP_