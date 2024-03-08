//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_PARENT_FROM_MEMBER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_PARENT_FROM_MEMBER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif 
#
#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif 

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/parent_from_member.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

//! Given a pointer to a member and its corresponding pointer to data member,
//! this function returns the pointer of the parent containing that member.
//! Note: this function does not work with pointer to members that rely on
//! virtual inheritance.
template<class Parent, class Member>
AUTODDS_INTRUSIVE_FORCEINLINE Parent *get_parent_from_member(Member *member, const Member Parent::* ptr_to_member) AUTODDS_NOEXCEPT
{  return ::autodds::libs::intrusive::detail::parent_from_member(member, ptr_to_member);  }

//! Given a const pointer to a member and its corresponding const pointer to data member,
//! this function returns the const pointer of the parent containing that member.
//! Note: this function does not work with pointer to members that rely on
//! virtual inheritance.
template<class Parent, class Member>
AUTODDS_INTRUSIVE_FORCEINLINE const Parent *get_parent_from_member(const Member *member, const Member Parent::* ptr_to_member) AUTODDS_NOEXCEPT
{  return ::autodds::libs::intrusive::detail::parent_from_member(member, ptr_to_member);  }

} // namespace intrusive 
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_PARENT_FROM_MEMBER_HPP_