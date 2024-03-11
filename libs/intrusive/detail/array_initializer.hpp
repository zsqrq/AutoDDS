//
// Created by wz on 24-3-11.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/config/config.hpp"
#include "libs/intrusive/detail/workaround.hpp"
#include "libs/move/detail/placement_new.hpp"
#include "libs/move/detail/force_ptr.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

union max_align
{
  char        char_;
  short       short_;
  int         int_;
  long        long_;
#ifdef AUTODDS_HAS_LONG_LONG
  ::autodds::long_long_type  long_long_;
#endif
  float       float_;
  double      double_;
  long double long_double_;
  void *      void_ptr_;
};

template<typename T, std::size_t N>
class array_initializer
{
 public:
  template<class CommonInitializer>
  array_initializer(const CommonInitializer& init)
  {
    char* init_buf = (char*)rawbuf;
    std::size_t i = 0;

    AUTODDS_INTRUSIVE_TRY{
        for(; i != N; ++i){
          ::new(init_buf, boost_move_new_t()) T(init);
          init_buf += sizeof(T);
        }
    }
    AUTODDS_INTRUSIVE_CATCH(...){
      while(i--){
        init_buf -= sizeof(T);
        move_detail::force_ptr<T*>(init_buf)->~T();
      }
      AUTODDS_INTRUSIVE_RETHROW;
    }
    AUTODDS_INTRUSIVE_CATCH_END
  }

  operator T* ()
  { return (T*)(rawbuf); }
  
 private:
  max_align rawbuf[(N * sizeof(T) - 1) / sizeof(max_align) + 1];
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP_