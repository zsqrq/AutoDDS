//
// Created by wz on 24-3-12.
//

#ifndef AUTODDS_LIBS_CONTAINER_DETAIL_VERSION_TYPE_HPP_
#define AUTODDS_LIBS_CONTAINER_DETAIL_VERSION_TYPE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/container/detail/config_begin.hpp"
#include "libs/container/detail/workaround.hpp"
#include "libs/container/detail/mpl.hpp"
#include "libs/container/detail/type_traits.hpp"

namespace autodds {
namespace libs {
namespace container {
namespace dtl {

template <class T, unsigned V>
struct version_type
    : public dtl::integral_constant<unsigned, V>
{
  typedef T type;
};

namespace impl{

template <class T>
struct extract_version
{
  typedef typename T::version type;
};

template <class T>
struct has_version
{
 private:
  struct two {char _[2];};
  template <class U> static two test(...);
  template <class U> static char test(const typename U::version*);
 public:
  static const bool value = sizeof(test<T>(0)) == 1;
  void dummy(){}
};

template <class T, bool = has_version<T>::value>
struct version
{
  static const unsigned value = 1;
};

template <class T>
struct version<T, true>
{
  static const unsigned value = extract_version<T>::type::value;
};

}  //namespace impl

template <class T>
struct version
    : public dtl::integral_constant<unsigned, impl::version<T>::value>
{};

template<class T, unsigned N>
struct is_version
{
  static const bool value =
      is_same< typename version<T>::type, integral_constant<unsigned, N> >::value;
};

} // namespace impl

typedef dtl::integral_constant<unsigned, 0> version_0;
typedef dtl::integral_constant<unsigned, 1> version_1;
typedef dtl::integral_constant<unsigned, 2> version_2;

}
}
}

#endif //AUTODDS_LIBS_CONTAINER_DETAIL_VERSION_TYPE_HPP_