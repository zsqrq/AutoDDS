//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/container/container_fwd.hpp"
#include "libs/move/detail/to_raw_pointer.hpp"
#include "libs/container/detail/transform_iterator.hpp"
#include "libs/container/detail/type_traits.hpp"
#include "libs/container/detail/placement_new.hpp"
#include "libs/intrusive/slist.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/twin.hpp"
#include "libs/move/utility_core.hpp"

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

template<typename VoidPointer>
class basic_multiallocation_chain
{
 private:
  typedef ati::slist_base_hook<ati::void_pointer<VoidPointer>,
                               ati::link_mode<ati::normal_link>>  node;
  typedef typename autodds::libs::intrusive::pointer_traits
      <VoidPointer>::template rebind_pointer<char>::type          char_ptr;
  typedef typename autodds::libs::intrusive::pointer_traits
      <char_ptr>::difference_type                                 difference_type;

  typedef ati::slist<node,
                     ati::linear<true>,
                     ati::cache_last<true>,
                     ati::size_type<typename make_unsigned<difference_type>::type>
  >                                            slist_impl_t;

  slist_impl_t   slist_impl_;

  typedef typename autodds::libs::intrusive::pointer_traits
      <VoidPointer>::template rebind_pointer<node>::type          node_ptr;
  typedef typename autodds::libs::intrusive::
  pointer_traits<node_ptr>                                    node_ptr_traits;

  static node& to_node(const VoidPointer& ptr)
  {
    return *static_cast<node*>(static_cast<void*>(autodds::libs::movelib::to_raw_pointer(ptr)));
  }

  static VoidPointer from_node(node& node)
  {
    return node_ptr_traits::pointer_to(node);
  }

  static node_ptr to_node_ptr(const VoidPointer& ptr)
  {
    return node_ptr_traits::static_cast_from(ptr);
  }

 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(basic_multiallocation_chain)

 public:

  typedef VoidPointer                           void_pointer;
  typedef typename slist_impl_t::iterator       iterator;
  typedef typename slist_impl_t::size_type      size_type;
  typedef typename autodds::libs::intrusive::twin<void_pointer>  pointer_pair;

  basic_multiallocation_chain() : slist_impl_()
  {}

  basic_multiallocation_chain(const void_pointer &b, const void_pointer &before_e, size_type n)
      :  slist_impl_(to_node_ptr(b), to_node_ptr(before_e), n)
  {}

  basic_multiallocation_chain(AUTODDS_RV_REF(basic_multiallocation_chain) other)
      :  slist_impl_(::autodds::libs::move(other.slist_impl_))
  {}

  basic_multiallocation_chain& operator=(AUTODDS_RV_REF(basic_multiallocation_chain) other)
  {
    slist_impl_ = ::autodds::libs::move(other.slist_impl_);
    return *this;
  }

  bool empty() const
  {  return slist_impl_.empty(); }

  size_type size() const
  {  return slist_impl_.size();  }

  iterator before_begin()
  {  return slist_impl_.before_begin(); }

  iterator begin()
  {  return slist_impl_.begin(); }

  iterator end()
  {  return slist_impl_.end(); }

  iterator last()
  {  return slist_impl_.last(); }

  void clear()
  {  slist_impl_.clear(); }

  iterator insert_after(iterator it, void_pointer m)
  {  return slist_impl_.insert_after(it, to_node(m));   }

  void push_front(const void_pointer &m)
  {  return slist_impl_.push_front(to_node(m));  }

  void push_back(const void_pointer &m)
  {  return slist_impl_.push_back(to_node(m));   }

  void_pointer pop_front()
  {
    node & n = slist_impl_.front();
    void_pointer ret = from_node(n);
    slist_impl_.pop_front();
    return ret;
  }

  void splice_after(iterator after_this, basic_multiallocation_chain &x, iterator before_b, iterator before_e, size_type n)
  {  slist_impl_.splice_after(after_this, x.slist_impl_, before_b, before_e, n);   }

  void splice_after(iterator after_this, basic_multiallocation_chain &x)
  {  slist_impl_.splice_after(after_this, x.slist_impl_);   }

  void erase_after(iterator before_b, iterator e, size_type n)
  {  slist_impl_.erase_after(before_b, e, n);   }

  void_pointer incorporate_after(iterator after_this, const void_pointer &b, size_type unit_bytes, size_type num_units)
  {
    typedef typename autodds::libs::intrusive::pointer_traits<char_ptr> char_pointer_traits;
    char_ptr elem = char_pointer_traits::static_cast_from(b);
    if(num_units){
      char_ptr prev_elem = elem;
      elem += difference_type(unit_bytes);
      for(size_type i = 0; i != num_units-1u; ++i, elem += difference_type(unit_bytes)){
        ::new (autodds::libs::movelib::to_raw_pointer(prev_elem), autodds_container_new_t()) void_pointer(elem);
        prev_elem = elem;
      }
      slist_impl_.incorporate_after(after_this, to_node_ptr(b), to_node_ptr(prev_elem), num_units);
    }
    return elem;
  }

  void incorporate_after(iterator after_this, void_pointer b, void_pointer before_e, size_type n)
  {  slist_impl_.incorporate_after(after_this, to_node_ptr(b), to_node_ptr(before_e), n);   }

  void swap(basic_multiallocation_chain &x)
  {  slist_impl_.swap(x.slist_impl_);   }

  static iterator iterator_to(const void_pointer &p)
  {  return slist_impl_t::s_iterator_to(to_node(p));   }

  pointer_pair extract_data()
  {
    if(AUTODDS_LIKELY(!slist_impl_.empty())){
      pointer_pair ret
          (slist_impl_.begin().operator->()
              ,slist_impl_.last().operator->());
      slist_impl_.clear();
      return ret;
    }
    else {
      return pointer_pair();
    }
  }

};

template<typename T>
struct cast_functor
{
  typedef typename dtl::add_reference<T>::type result_type;
  template<class U>
  result_type operator()(U& ptr) const
  {
    return *static_cast<T*>(static_cast<void*>(&ptr));
  }
};

template<typename MultiallocationChain, typename T>
class transform_multiallocation_chain : public MultiallocationChain
{
 private:
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(transform_multiallocation_chain)

  typedef typename MultiallocationChain::void_pointer        void_pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <void_pointer>                                         void_pointer_traits;
  typedef typename void_pointer_traits::template
  rebind_pointer<T>::type                                    pointer;
  typedef typename autodds::libs::intrusive::pointer_traits
      <pointer>                                              pointer_traits;

  static pointer cast(const void_pointer& ptr)
  {
    return pointer_traits::static_cast_from(ptr);
  }

 public:
  typedef transform_iterator
      < typename MultiallocationChain::iterator
          , dtl::cast_functor <T> >                          iterator;
  typedef typename MultiallocationChain::size_type           size_type;
  typedef autodds::libs::intrusive::twin<pointer>            pointer_pair;

  transform_multiallocation_chain()
      : MultiallocationChain()
  {}

  transform_multiallocation_chain(AUTODDS_RV_REF(transform_multiallocation_chain) other)
      : MultiallocationChain(::autodds::libs::move(static_cast<MultiallocationChain&>(other)))
  {}

  transform_multiallocation_chain(AUTODDS_RV_REF(MultiallocationChain) other)
      : MultiallocationChain(::autodds::libs::move(static_cast<MultiallocationChain&>(other)))
  {}

  transform_multiallocation_chain& operator=(AUTODDS_RV_REF(transform_multiallocation_chain) other)
  {
    return static_cast<MultiallocationChain&>
    (this->MultiallocationChain::operator=(::autodds::libs::move(static_cast<MultiallocationChain&>(other))));
  }

  void push_front(const pointer &mem)
  {   this->MultiallocationChain::push_front(mem);  }

  void push_back(const pointer &mem)
  {  return this->MultiallocationChain::push_back(mem);   }

  void swap(transform_multiallocation_chain &other_chain)
  {  this->MultiallocationChain::swap(other_chain); }

  void splice_after(iterator after_this, transform_multiallocation_chain &x, iterator before_b, iterator before_e, size_type n)
  {  this->MultiallocationChain::splice_after(after_this.base(), x, before_b.base(), before_e.base(), n);  }

  void incorporate_after(iterator after_this, pointer b, pointer before_e, size_type n)
  {  this->MultiallocationChain::incorporate_after(after_this.base(), b, before_e, n);  }

  pointer pop_front()
  {  return cast(this->MultiallocationChain::pop_front());  }

  bool empty() const
  {  return this->MultiallocationChain::empty(); }

  iterator before_begin()
  {  return iterator(this->MultiallocationChain::before_begin());   }

  iterator begin()
  {  return iterator(this->MultiallocationChain::begin());   }

  iterator last()
  {  return iterator(this->MultiallocationChain::last());  }

  iterator end()
  {  return iterator(this->MultiallocationChain::end());   }

  size_type size() const
  {  return this->MultiallocationChain::size();  }

  void clear()
  {  this->MultiallocationChain::clear(); }

  iterator insert_after(iterator it, pointer m)
  {  return iterator(this->MultiallocationChain::insert_after(it.base(), m)); }

  static iterator iterator_to(const pointer &p)
  {  return iterator(MultiallocationChain::iterator_to(p));  }

  pointer_pair extract_data()
  {
    typename MultiallocationChain::pointer_pair data(this->MultiallocationChain::extract_data());
    return pointer_pair(cast(data.first), cast(data.second));
  }

};

} // namespace dtl
} // namespace container
} // namespace libs
} // namespace autodds


#endif //AUTODDS_LIBS_CONTAINER_DETAIL_MULTIALLOCATION_CHAIN_HPP_