//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_ALGO_MOVE_HPP_
#define AUTODDS_LIBS_MOVE_ALGO_MOVE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/utility_core.hpp"
#include "libs/move/detail/iterator_traits.hpp"
#include "libs/move/detail/iterator_to_raw_pointer.hpp"
#include "libs/move/detail/addressof.hpp"

#if !defined(AUTODDS_MOVE_NOT_USE_STANDARD_LIBRARY_MOVE)
#include <algorithm>
#endif

namespace autodds {
namespace libs {

#if defined(AUTODDS_MOVE_NOT_USE_STANDARD_LIBRARY_MOVE)

template <typename I, // I models BidirectionalIterator
    typename O> // O models BidirectionalIterator
O move_backward(I f, I l, O result)
{
  while (f != l) {
    --l; --result;
    *result = ::autodds::libs::move(*l);
  }
  return result;
}
#else

using ::std::move_backward;
#endif

template <typename I, typename F>
F uninitialized_move(I f, I l, F r)
{
  typedef typename autodds::libs::movelib::iterator_traits<I>::value_type input_value_type;
  F back = r;
  AUTODDS_MOVE_TRY{
      while (f != l) {
        void* const addr = static_cast<void*>(autodds::libs::move_detail::addressof(*r));
        new(addr) input_value_type (autodds::libs::move(*f));
        ++f; ++r;
      }
  }
  AUTODDS_MOVE_CATCH(...) {
    for (; back != r; ++back) {
      autodds::libs::movelib::iterator_to_raw_pointer(back)->~input_value_type();
    }
    AUTODDS_MOVE_RETHROW
  }
  AUTODDS_MOVE_CATCH_END
  return r;
}


} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_ALGO_MOVE_HPP_