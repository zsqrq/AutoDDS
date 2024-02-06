//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/pointer_rebind.hpp"
#include "libs/move/detail/pointer_element.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include <cstddef>

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace intrusive {
namespace detail {

AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_pointer_to, pointer_to)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_dynamic_cast_from, dynamic_cast_from)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_static_cast_from, static_cast_from)
AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_const_cast_from, const_cast_from)

AUTODDS_INTRUSIVE_INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(element_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(difference_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(size_type)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(reference)
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(value_traits_ptr)

}
}
}

#endif //AUTODDS_LIBS_INTRUSIVE_POINTER_TRAITS_HPP_