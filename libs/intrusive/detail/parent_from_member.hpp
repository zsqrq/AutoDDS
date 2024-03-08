//
// Created by wz on 24-3-8.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_PARENT_FROM_MEMBER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_PARENT_FROM_MEMBER_HPP_

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

template<class Parent, class Member>
AUTODDS_INTRUSIVE_FORCEINLINE std::ptrdiff_t offset_from_pointer_to_member(const Member Parent::* ptr_to_member) 
{
  //The implementation of a pointer to member is compiler dependent.
  const Parent *const parent = 0;
  const char *const member = static_cast<const char *>(static_cast<const void *>(&(parent->*ptr_to_member)));
  return std::ptrdiff_t(member - static_cast<const char *>(static_cast<const void *>(parent)));
}

template<class Parent, class Member>
AUTODDS_INTRUSIVE_FORCEINLINE Parent *parent_from_member(Member *member, const Member Parent::* ptr_to_member)
{
return static_cast<Parent*>
(
static_cast<void*>
(
static_cast<char*>(static_cast<void*>(member)) - offset_from_pointer_to_member(ptr_to_member)
)
);
}

template<class Parent, class Member>
AUTODDS_INTRUSIVE_FORCEINLINE const Parent *parent_from_member(const Member *member, const Member Parent::* ptr_to_member)
{
  return static_cast<const Parent*>
  (
      static_cast<const void*>
      (
          static_cast<const char*>(static_cast<const void*>(member)) - offset_from_pointer_to_member(ptr_to_member)
      )
  );
}

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_PARENT_FROM_MEMBER_HPP_