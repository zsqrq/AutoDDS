//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

template<class Container, class Disposer>
class exception_disposer
{
  Container *cont_;
  Disposer  &disp_;

  exception_disposer(const exception_disposer&);
  exception_disposer &operator=(const exception_disposer&);

 public:
  exception_disposer(Container &cont, Disposer &disp)
      :  cont_(&cont), disp_(disp)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE void release()
  {  cont_ = 0;  }

  ~exception_disposer()
  {
    if(cont_){
      cont_->clear_and_dispose(disp_);
    }
  }
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP_