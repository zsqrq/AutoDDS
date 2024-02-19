//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_DETAIL_IS_FUNCTION_CXX_11_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_DETAIL_IS_FUNCTION_CXX_11_HPP_

#include "libs/type_traits/integral_constant.hpp"

namespace autodds {
namespace libs {

template <typename T>
struct is_function : public false_type {};

#if defined(__cpp_noexcept_function_type) && !defined(AUTODDS_TT_NO_DEDUCED_NOEXCEPT_PARAM)
#define AUTODDS_TT_NOEXCEPT_PARAM , bool NE
#define AUTODDS_TT_NOEXCEPT_DECL noexcept(NE)
#else
#define AUTODDS_TT_NOEXCEPT_PARAM
#define AUTODDS_TT_NOEXCEPT_DECL
#endif
#define AUTODDS_TT_DEF_CALL

template <typename Ret, typename... Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...) AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <typename Ret, typename... Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...) AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <typename Ret, typename... Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <typename Ret, typename... Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...) const AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)volatile AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)volatile AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const volatile AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)const volatile AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)& AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)& AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)const & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)volatile & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)volatile & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const volatile & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)const volatile & AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)&& AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)&& AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};
template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)const && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)volatile && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};
template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)volatile && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

template <class Ret, class...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret AUTODDS_TT_DEF_CALL(Args...)const volatile && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};
template <class Ret, class ...Args AUTODDS_TT_NOEXCEPT_PARAM>
struct is_function<Ret(Args..., ...)const volatile && AUTODDS_TT_NOEXCEPT_DECL> : public true_type {};

}
}

#undef AUTODDS_TT_NOEXCEPT_DECL
#undef AUTODDS_TT_NOEXCEPT_PARAM
#undef AUTODDS_TT_DEF_CALL

#endif //AUTODDS_LIBS_TYPE_TRAITS_DETAIL_IS_FUNCTION_CXX_11_HPP_