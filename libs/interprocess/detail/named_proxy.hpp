//
// Created by wz on 24-3-5.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_NAMED_PROXY_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_NAMED_PROXY_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/in_place_interface.hpp"
#include "libs/interprocess/detail/mpl.hpp"
#include "libs/move/utility_core.hpp"

#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/variadic_templates_tools.hpp"
#include "libs/container/detail/placement_new.hpp"

#include <cstddef>

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <typename T, bool is_iterator, typename... Args>
struct CtorArgN : public placement_destroy<T>
{
  typedef bool_<is_iterator>  IsIterator;
  typedef CtorArgN<T, is_iterator, Args...> self_t;
  typedef typename build_number_seq<sizeof...(Args)>::type index_tuple_t;

  self_t& operator ++()
  {
    this->do_increment(IsIterator(), index_tuple_t());
    return *this;
  }

  self_t operator ++(int)
  {
    return ++*this;
  }

  virtual void construc_n(void* mem, std::size_t num,
                          std::size_t& constructed) AUTODDS_OVERRIDE
  {
    T* memory = static_cast<T*>(mem);
    for (constructed = 0; constructed < num; ++constructed)
    {
      this->construct(memory++, IsIterator(), index_tuple_t());
      this->do_increment(IsIterator(), index_tuple_t());
    }
  }

 private:
  template<std::size_t... IdxPack>
  void construct(void* mem, true_, const index_tuple<IdxPack...>&)
  {
    ::new((void*)mem, autodds_container_new_t())T(*autodds::libs::forward<Args>(get<IdxPack>(args_))...);
  }

  template<std::size_t... IdxPack>
  void construct(void* mem, false_, const index_tuple<IdxPack...>&)
  {
    ::new((void*)mem, autodds_container_new_t())T(autodds::libs::forward<Args>(get<IdxPack>(args_))...);
  }

  template<std::size_t... IdxPack>
  void do_increment(true_, const index_tuple<IdxPack...>&)
  {
    this->expansion_helper(++get<IdxPack>(args_)...);
  }

  template<class ...ExpansionArgs>
  void expansion_helper(ExpansionArgs &&...)
  {}

  template<std::size_t ...IdxPack>
  void do_increment(false_, const index_tuple<IdxPack...>&)
  {}

  tuple<Args&...> args_;
};

//!Describes a proxy class that implements named
//!allocation syntax.
template <typename SegmentManager, typename T, bool is_iterator>
class named_proxy
{
  typedef typename SegmentManager::char_type  char_type;
  const char_type *     mp_name;
  SegmentManager*       mp_manager;
  mutable std::size_t   m_num;
  const bool            m_find;
  const bool            m_dothrow;

 public:
  named_proxy(SegmentManager* manager, const char_type* name, bool find, bool dothrow)
      : mp_name(name), mp_manager(manager), m_num(1), m_find(find), m_dothrow(dothrow)
  {}

  template<class... Args>
  T* operator ()(Args&& ...args) const
  {
    CtorArgN<T, is_iterator, Args...>&& ctor_obj = CtorArgN<T, is_iterator, Args...>
        (autodds::libs::forward<Args>(args)...);
    return mp_manager->template
        generic_construct<T>(mp_name, m_num, m_find, m_dothrow, ctor_obj);
  }

  const named_proxy& operator [](std::size_t num) const
  { m_num *= num; return *this; }
};

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_NAMED_PROXY_HPP_