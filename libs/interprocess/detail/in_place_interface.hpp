//
// Created by wz on 24-3-4.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_IN_PLACE_INTERFACE_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_IN_PLACE_INTERFACE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/container/detail/type_traits.hpp"
#include <typeinfo>

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

struct in_place_interface {

  in_place_interface(std::size_t alignm, std::size_t sz, const char *tname)
      : alignment(alignm), size(sz), type_name(tname)
  {}

  std::size_t alignment;
  std::size_t size;
  const char* type_name;

  virtual void construct_n(void *mem, std::size_t num, std::size_t &constructed) = 0;
  virtual void destroy_n(void *mem, std::size_t num, std::size_t &destroyed) = 0;
  virtual ~in_place_interface(){}
};

template <typename T>
struct placement_destroy : public in_place_interface
{
  placement_destroy()
      : in_place_interface(autodds::libs::container::dtl::alignment_of<T>::value, sizeof(T), typeid(T).name())
  {}

  virtual void destroy_n(void* mem, std::size_t num, std::size_t& destroyed) AUTODDS_OVERRIDE
  {
    T* memory = static_cast<T*>(mem);
    for (destroyed = 0; destroyed < num; ++destroyed) {
      (memory++)->~T();
    }
  }

  virtual void construct_n(void *, std::size_t, std::size_t &) AUTODDS_OVERRIDE {}

 private:
  void destroy(void* mem)
  {
    static_cast<T*>(mem)->~T();
  }
};

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_IN_PLACE_INTERFACE_HPP_