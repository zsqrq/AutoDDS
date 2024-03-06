//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_LINK_MODE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_LINK_MODE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {

enum link_mode_type {
  //!If this linking policy is specified in a value_traits class
  //!as the link_mode, containers
  //!configured with such value_traits won't set the hooks
  //!of the erased values to a default state. Containers also won't
  //!check that the hooks of the new values are default initialized.
  normal_link,

  //!If this linking policy is specified in a value_traits class
  //!as the link_mode, containers
  //!configured with such value_traits will set the hooks
  //!of the erased values to a default state. Containers also will
  //!check that the hooks of the new values are default initialized.
  safe_link,

  //!Same as "safe_link" but the user type is an auto-unlink
  //!type, so the containers with constant-time size features won't be
  //!compatible with value_traits configured with this policy.
  //!Containers also know that the a value can be silently erased from
  //!the container without using any function provided by the containers.
  auto_unlink
};

template <link_mode_type link_mode>
struct is_safe_autounlink
{
  static const bool value = (int)link_mode == (int)auto_unlink ||
      (int)link_mode == (int)safe_link;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_LINK_MODE_HPP_