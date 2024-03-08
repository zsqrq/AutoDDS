//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_HOOK_TRAITS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_HOOK_TRAITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_traits.hpp"
#include "libs/intrusive/detail/parent_from_member.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/move/detail/to_raw_pointer.hpp"
#include "libs/intrusive/detail/node_holder.hpp"
#include "libs/common/assert.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

template<class T, class NodePtr, class Tag, unsigned int Type>
struct bhtraits_base
{
 public:
  typedef NodePtr                                                   node_ptr;
  typedef typename pointer_traits<node_ptr>::element_type           node;
  typedef node_holder<node, Tag, Type>                              node_holder_type;
  typedef T                                                         value_type;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<const node>::type                         const_node_ptr;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<T>::type                                  pointer;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<const T>::type                            const_pointer;
  //typedef typename pointer_traits<pointer>::reference             reference;
  //typedef typename pointer_traits<const_pointer>::reference       const_reference;
  typedef T &                                                       reference;
  typedef const T &                                                 const_reference;
  typedef node_holder_type &                                        node_holder_reference;
  typedef const node_holder_type &                                  const_node_holder_reference;
  typedef node&                                                     node_reference;
  typedef const node &                                              const_node_reference;

  AUTODDS_INTRUSIVE_FORCEINLINE static pointer to_value_ptr(node_ptr n)
  {
    pointer p = pointer_traits<pointer>::pointer_to
        (static_cast<reference>(static_cast<node_holder_reference>(*n)));
    AUTODDS_ASSERT(!!p);
    return p;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static const_pointer to_value_ptr(const_node_ptr n)
  {
    const_pointer p = pointer_traits<const_pointer>::pointer_to
        (static_cast<const_reference>(static_cast<const_node_holder_reference>(*n)));
    AUTODDS_ASSERT(!!p);
    return p;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr to_node_ptr(reference value)
  {
    node_ptr p = pointer_traits<node_ptr>::pointer_to
        (static_cast<node_reference>(static_cast<node_holder_reference>(value)));
    AUTODDS_ASSERT(!!p);
    return p;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static const_node_ptr to_node_ptr(const_reference value)
  {
    const_node_ptr p = pointer_traits<const_node_ptr>::pointer_to
        (static_cast<const_node_reference>(static_cast<const_node_holder_reference>(value)));
    AUTODDS_ASSERT(!!p);
    return p;
  }
};

template<class T, class NodeTraits, link_mode_type LinkMode, class Tag, unsigned int Type>
struct bhtraits
    : public bhtraits_base<T, typename NodeTraits::node_ptr, Tag, Type>
{
  static const link_mode_type link_mode = LinkMode;
  typedef NodeTraits node_traits;
};


template<class T, class Hook, Hook T::* P>
struct mhtraits
{
 public:
  typedef Hook                                                      hook_type;
  typedef typename hook_type::hooktags::node_traits                 node_traits;
  typedef typename node_traits::node                                node;
  typedef T                                                         value_type;
  typedef typename node_traits::node_ptr                            node_ptr;
  typedef typename node_traits::const_node_ptr                      const_node_ptr;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<T>::type                               pointer;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<const T>::type                         const_pointer;
  typedef T &                                                       reference;
  typedef const T &                                                 const_reference;
  typedef node&                                                     node_reference;
  typedef const node &                                              const_node_reference;
  typedef hook_type&                                                hook_reference;
  typedef const hook_type &                                         const_hook_reference;

  static const link_mode_type link_mode = Hook::hooktags::link_mode;

  AUTODDS_INTRUSIVE_FORCEINLINE static node_ptr to_node_ptr(reference value)
  {
    return pointer_traits<node_ptr>::pointer_to
        (static_cast<node_reference>(static_cast<hook_reference>(value.*P)));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static const_node_ptr to_node_ptr(const_reference value)
  {
    return pointer_traits<const_node_ptr>::pointer_to
        (static_cast<const_node_reference>(static_cast<const_hook_reference>(value.*P)));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static pointer to_value_ptr(node_ptr n)
  {
    return pointer_traits<pointer>::pointer_to
        (*detail::parent_from_member<T, Hook>
            (static_cast<Hook*>(autodds::libs::movelib::to_raw_pointer(n)), P));
  }

  AUTODDS_INTRUSIVE_FORCEINLINE static const_pointer to_value_ptr(const_node_ptr n)
  {
    return pointer_traits<const_pointer>::pointer_to
        (*detail::parent_from_member<T, Hook>
            (static_cast<const Hook*>(autodds::libs::movelib::to_raw_pointer(n)), P));
  }
};

template<class Functor>
struct fhtraits
{
 public:
  typedef typename Functor::hook_type                               hook_type;
  typedef typename Functor::hook_ptr                                hook_ptr;
  typedef typename Functor::const_hook_ptr                          const_hook_ptr;
  typedef typename hook_type::hooktags::node_traits                 node_traits;
  typedef typename node_traits::node                                node;
  typedef typename Functor::value_type                              value_type;
  typedef typename node_traits::node_ptr                            node_ptr;
  typedef typename node_traits::const_node_ptr                      const_node_ptr;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<value_type>::type                      pointer;
  typedef typename pointer_traits<node_ptr>::
  template rebind_pointer<const value_type>::type                const_pointer;
  typedef value_type &                                              reference;
  typedef const value_type &                                        const_reference;
  static const link_mode_type link_mode = hook_type::hooktags::link_mode;

  static node_ptr to_node_ptr(reference value)
  {  return static_cast<node*>(autodds::libs::movelib::to_raw_pointer(Functor::to_hook_ptr(value)));  }

  static const_node_ptr to_node_ptr(const_reference value)
  {  return static_cast<const node*>(autodds::libs::movelib::to_raw_pointer(Functor::to_hook_ptr(value)));  }

  static pointer to_value_ptr(node_ptr n)
  {  return Functor::to_value_ptr(to_hook_ptr(n));  }

  static const_pointer to_value_ptr(const_node_ptr n)
  {  return Functor::to_value_ptr(to_hook_ptr(n));  }

 private:
  static hook_ptr to_hook_ptr(node_ptr n)
  {  return hook_ptr(&*static_cast<hook_type*>(&*n));  }

  static const_hook_ptr to_hook_ptr(const_node_ptr n)
  {  return const_hook_ptr(&*static_cast<const hook_type*>(&*n));  }
};


} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_HOOK_TRAITS_HPP_