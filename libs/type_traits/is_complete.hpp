//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_IS_COMPLETE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_IS_COMPLETE_HPP_

#include "libs/type_traits/declval.hpp"
#include "libs/type_traits/integral_constant.hpp"
#include "libs/type_traits/remove_reference.hpp"
#include "libs/type_traits/detail/yes_no_type.hpp"
#include "libs/config/workaround.hpp"

#ifdef AUTODDS_TT_HAS_ACCURATE_IS_FUNCTION
#include "libs/type_traits/detail/is_function_cxx_11.hpp"
#endif

namespace autodds {
namespace libs {

#define AUTODDS_TT_HAS_WORKING_IS_COMPLETE
#if !defined(AUTODDS_NO_SFINAE_EXPR) && !AUTODDS_WORKAROUND(AUTODDS_MSVC, <= 1900) && !AUTODDS_WORKAROUND(AUTODDS_GCC_VERSION, < 40600)

namespace detail{

template <std::size_t N>
struct ok_tag { double d; char c[N]; };

template <class T>
ok_tag<sizeof(T)> check_is_complete(int);
template <class T>
char check_is_complete(...);
}

template <class T> struct is_complete
    : public integral_constant<bool,
                               ::autodds::libs::is_function<typename autodds::libs::remove_reference<T>::type>::value
                               || (sizeof(check_is_complete<T>(0)) != sizeof(char))> {};

#elif !defined(AUTODDS_NO_SFINAE) && !defined(AUTODDS_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) && !AUTODDS_WORKAROUND(AUTODDS_GCC_VERSION, < 40500)

namespace detail
   {

      template <class T>
      struct is_complete_imp
      {
         template <class U, class = decltype(sizeof(boost::declval< U >())) >
         static type_traits::yes_type check(U*);

         template <class U>
         static type_traits::no_type check(...);

         static const bool value = sizeof(check<T>(0)) == sizeof(type_traits::yes_type);
      };

} // namespace detail


   template <class T>
   struct is_complete : boost::integral_constant<bool, ::boost::is_function<typename boost::remove_reference<T>::type>::value || ::boost::detail::is_complete_imp<T>::value>
   {};
   template <class T>
   struct is_complete<T&> : boost::is_complete<T> {};

#else

template <class T> struct is_complete
    : public boost::integral_constant<bool, true> {};

#undef AUTODDS_TT_HAS_WORKING_IS_COMPLETE

#endif

}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_IS_COMPLETE_HPP_