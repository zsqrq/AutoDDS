//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_ITERATOR_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/iterator.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/container/detail/mpl.hpp"

namespace autodds {
namespace libs {
namespace container {

using ::autodds::libs::intrusive::iterator_traits;
using ::autodds::libs::intrusive::iter_difference;
using ::autodds::libs::intrusive::iter_category;
using ::autodds::libs::intrusive::iter_value;
using ::autodds::libs::intrusive::iter_size;
using ::autodds::libs::intrusive::iterator_distance;
using ::autodds::libs::intrusive::iterator_udistance;
using ::autodds::libs::intrusive::iterator_advance;
using ::autodds::libs::intrusive::iterator_uadvance;
using ::autodds::libs::intrusive::make_iterator_advance;
using ::autodds::libs::intrusive::make_iterator_uadvance;
using ::autodds::libs::intrusive::iterator;
using ::autodds::libs::intrusive::iterator_enable_if_tag;
using ::autodds::libs::intrusive::iterator_disable_if_tag;
using ::autodds::libs::intrusive::iterator_arrow_result;

template <typename Container>
class back_emplacer
{
 private:
  Container& container;

 public:
  typedef std::output_iterator_tag    iterator_category;
  typedef void                        value_type;
  typedef void                        difference_type;
  typedef void                        pointer;
  typedef void                        reference;

  back_emplacer(Container& other)
      : container(other)
  {}

  template<class U>
  back_emplacer& operator =( AUTODDS_FWD_REF(U) value)
  {
    container.emplace_back(autodds::libs::forward<U>(value));
    return *this;
  }
  
  back_emplacer& operator *() { return *this; }
  back_emplacer& operator ++() { return *this; }
  back_emplacer& operator ++(int) { return *this; }
};

#ifndef AUTODDS_CONTAINER_NO_CXX17_CTAD

template<class InputIterator>
using it_based_non_const_first_type_t = typename dtl::remove_const<typename iterator_traits<InputIterator>::value_type::first_type>::type;

template<class InputIterator>
using it_based_const_first_type_t = const typename dtl::remove_const<typename iterator_traits<InputIterator>::value_type::first_type>::type;

template<class InputIterator>
using it_based_second_type_t = typename iterator_traits<InputIterator>::value_type::second_type;

template<class InputIterator>
using it_based_value_type_t = typename iterator_traits<InputIterator>::value_type;

#endif

} // namespace container
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_ITERATOR_HPP_