//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_ITERATOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_ITERATOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
#include "libs/intrusive/detail/std_fwd.hpp"
#include "libs/intrusive/detail/workaround.hpp"
#include "libs/move/detail/iterator_traits.hpp"
#include "libs/move/detail/meta_utils.hpp"

namespace autodds {
namespace libs {
namespace iterators {

struct incrementable_traversal_tag;
struct single_pass_traversal_tag;
struct forward_traversal_tag;
struct bidirectional_traversal_tag;
struct random_access_traversal_tag;

namespace detail {

template <typename CateGory, typename Traversal>
struct iterator_category_with_traversal;

} // namespace detail

} // namespace iterators
} // namespace libs
} // namespace iterators

namespace autodds {
namespace libs {
namespace intrusive {

using autodds::libs::movelib::iterator_traits;
using autodds::libs::movelib::iter_difference;
using autodds::libs::movelib::iter_value;
using autodds::libs::movelib::iter_category;
using autodds::libs::movelib::iter_size;

template <typename Category, typename T, typename Difference, typename Pointer, typename Reference>
struct iterator
{
  typedef Category     iterator_category;
  typedef T            value_type;
  typedef Difference   difference_type;
  typedef Pointer      pointer;
  typedef Reference    reference;
};

template<typename Tag>
struct get_std_category_from_tag
{
  typedef Tag type;
};

template <typename Category>
struct get_std_category_from_tag
    <autodds::libs::iterators::detail::iterator_category_with_traversal
         <Category, autodds::libs::iterators::incrementable_traversal_tag> >
{
  typedef std::input_iterator_tag type;
};

template <typename Category>
struct get_std_category_from_tag
    <autodds::libs::iterators::detail::iterator_category_with_traversal
         <Category, autodds::libs::iterators::single_pass_traversal_tag> >
{
  typedef std::input_iterator_tag type;
};

template <typename Category>
struct get_std_category_from_tag
    <autodds::libs::iterators::detail::iterator_category_with_traversal
         <Category, autodds::libs::iterators::forward_traversal_tag> >
{
  typedef std::input_iterator_tag type;
};

template <typename Category>
struct get_std_category_from_tag
    <autodds::libs::iterators::detail::iterator_category_with_traversal
         <Category, autodds::libs::iterators::bidirectional_traversal_tag> >
{
  typedef std::bidirectional_iterator_tag type;
};

template <typename Category>
struct get_std_category_from_tag
    <autodds::libs::iterators::detail::iterator_category_with_traversal
         <Category, autodds::libs::iterators::random_access_traversal_tag> >
{
  typedef std::random_access_iterator_tag type;
};

template <typename It>
struct get_std_category_from_it
    : get_std_category_from_tag< typename autodds::libs::intrusive::iter_category<It>::type >
{};

template <typename I, typename Tag, typename R = void>
struct iterator_enable_if_tag
    : autodds::libs::move_detail::enable_if_c
          <autodds::libs::move_detail::is_same
               <typename get_std_category_from_it<I>::type, Tag>::value, R>
{};

template <typename I, typename Tag, typename R = void>
struct iterator_disable_if_tag
    : autodds::libs::move_detail::enable_if_c
          <!autodds::libs::move_detail::is_same
              <typename get_std_category_from_it<I>::type, Tag>::value, R>
{};

template <typename I, typename Tag, typename Tag2, typename R = void>
struct iterator_enable_if_convertible_tag : autodds::libs::move_detail::enable_if_c<
    autodds::libs::move_detail::is_same_or_convertible<
        typename get_std_category_from_it<I>::type, Tag>::value &&
        !autodds::libs::move_detail::is_same_or_convertible<
            typename get_std_category_from_it<I>::type, Tag2>::value, R>
{};

template <typename I, typename Tag>
struct iterator_enable_if_tag_difference_type
    : iterator_enable_if_tag<I, Tag, typename autodds::libs::intrusive::iter_difference<I>::type>
{};

template <typename I, typename Tag>
struct iterator_disable_if_tag_difference_type
    : iterator_disable_if_tag<I, Tag, typename autodds::libs::intrusive::iter_difference<I>::type>
{};

// advance
template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::input_iterator_tag>::type
iterator_advance(InputIt& it, typename iter_difference<InputIt>::type n)
{
  while (n--)
  { ++it; }
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::forward_iterator_tag>::type
iterator_advance(InputIt& it, typename iter_difference<InputIt>::type n)
{
  while (n--)
  { ++it; }
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::bidirectional_iterator_tag>::type
iterator_advance(InputIt& it, typename iter_difference<InputIt>::type n)
{
  for ( ; 0 < n; --n) {
    ++it;
  }
  for (; n < 0; ++n) {
    --it;
  }
}

template <typename InputIt, typename Distance>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::random_access_iterator_tag>::type
iterator_advance(InputIt& it, Distance n)
{ it += n; }

template <typename InputIt, typename Distance>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::random_access_iterator_tag, InputIt>::type
make_iterator_advance(InputIt it, Distance n )
{
  (iterator_advance)(it, n);
  return it;
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE void iterator_uadvance(InputIt& it, typename iter_size<InputIt>::type n)
{
  (iterator_advance)(it, (typename iterator_traits<InputIt>::difference_type)n);
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE InputIt make_iterator_uadvance(InputIt it, typename iter_size<InputIt>::type n)
{
  (iterator_uadvance)(it, n);
  return it;
}

//    iterator_distance
template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE
iterator_disable_if_tag_difference_type<InputIt, std::random_access_iterator_tag>::type
iterator_distance(InputIt first, InputIt last)
{
  typename iter_difference<InputIt>::type off = 0;
  while (first != last) {
    ++off;
    ++first;
  }
  return off;
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag_difference_type
    <InputIt, std::random_access_iterator_tag>::type
iterator_distance(InputIt first, InputIt last)
{
  typename iter_difference<InputIt>::type off = last - first;
  return off;
}

//    iterator_udistance
template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iter_size<InputIt>::type
iterator_udistance(InputIt first, InputIt last)
{
  return (typename iter_size<InputIt>::type)(iterator_distance)(first, last);
}

//    iterator_next
template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE InputIt iterator_next(InputIt it, typename iter_difference<InputIt>::type n)
{
  (iterator_advance)(it, n);
  return it;
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE InputIt iterator_unext(InputIt it, typename iterator_traits<InputIt>::size_type n)
{
  (iterator_uadvance)(it, n);
  return it;
}

// iterator_arrow_result
template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE typename iterator_traits<InputIt>::pointer iterator_arrow_result(const InputIt& i)
{
  return i.operator->();
}

template <typename InputIt>
AUTODDS_INTRUSIVE_FORCEINLINE InputIt * iterator_arrow_result(InputIt *p)
{  return p;   }

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_ITERATOR_HPP_