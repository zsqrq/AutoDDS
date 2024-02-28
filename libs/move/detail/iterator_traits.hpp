//
// Created by wz on 24-2-28.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TRAITS_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TRAITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if (AUTODDS_CXX_VERSION > 201703L) && defined(__cpp_lib_concepts)

#include <iterator>
#define AUTODDS_MOVE_CONTIGUOUS_ITERATOR_TAG

namespace autodds {
namespace libs {
namespace movelib {

using std::iterator_traits;

template<typename T>
struct iter_difference
{
  typedef typename std::iterator_traits<T>::difference_type type;
};

template <typename T>
struct iter_value
{
  typedef typename std::iterator_traits<T>::value_type type;
};

template <typename T>
struct iter_category
{
  typedef typename std::iterator_traits<T>::iterator_category type;
};

} // namespace movelib
} // namespace libs
} // namespace autodds

#else
#include <cstddef>
#include "libs/move/detail/type_traits.hpp"
#include "libs/move/detail/std_ns_begin.hpp"

AUTODDS_MOVE_STD_NS_BEG

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;
struct output_iterator_tag;

#if (  (defined(AUTODDS_GNU_STDLIB) && (__cplusplus > 201703L))\
    || (defined(_LIBCPP_VERSION) && (_LIBCPP_STD_VER > 17))\
    || (defined(_YVALS) && defined(_CPPLIB_VER) && defined(__cpp_lib_concepts))\
    || (__cplusplus >= 202002L)\
    )
#  define AUTODDS_MOVE_CONTIGUOUS_ITERATOR_TAG
struct contiguous_iterator_tag;

#endif
AUTODDS_MOVE_STD_NS_END
#include "libs/move/detail/std_ns_end.hpp"

namespace autodds {
namespace libs {
namespace movelib {

template <typename T>
struct iter_difference {
  typedef typename T::difference_type type;
};

template <typename T>
struct iter_difference<T*>
{
  typedef std::ptrdiff_t type;
};

template <typename T>
struct iter_value
{
  typedef typename T::value_type type;
};

template <typename T>
struct iter_value<T*>
{
  typedef T type;
};

template <typename T>
struct iter_value<const T*>
{
  typedef T type;
};

template <typename T>
struct iter_category
{
  typedef typename T::iterator_category type;
};


template <typename T>
struct iter_category<T*>
{
  typedef std::random_access_iterator_tag type;
};

template <typename Iterator>
struct iterator_traits
{
  typedef typename iter_difference<Iterator>::type        difference_type;
  typedef typename iter_value<Iterator>::type             value_type;
  typedef typename Iterator::pointer                      pointer;
  typedef typename Iterator::reference                    reference;
  typedef typename iter_category<Iterator>::type          iterator_category;
};

template <typename T>
struct iterator_traits<T*>
{
  typedef std::ptrdiff_t                    difference_type;
  typedef T                                 value_type;
  typedef T*                                pointer;
  typedef T&                                reference;
  typedef std::random_access_iterator_tag   iterator_category;
};

template <typename T>
struct iterator_traits<const T*>
{
  typedef std::ptrdiff_t                    difference_type;
  typedef T                                 value_type;
  typedef const T*                          pointer;
  typedef const T&                          reference;
  typedef std::random_access_iterator_tag   iterator_category;
};

} // namespace autodds
} // namespace libs
} // namespace movelib

#endif

#include "libs/move/detail/type_traits.hpp"

namespace autodds {
namespace libs {
namespace movelib {

template <typename T>
struct iter_size :
    autodds::libs::move_detail::make_unsigned<typename iter_difference<T>::type>
{};

} // namespace movelib
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_MOVE_DETAIL_ITERATOR_TRAITS_HPP_