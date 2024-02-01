//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_PERMISSIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_PERMISSIONS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include <sys/stat.h>

namespace autodds {
namespace libs {
namespace interprocess {

//!The permissions class represents permissions to be set to shared memory or files
class permissions {

  typedef ::mode_t os_permissions_type;

  os_permissions_type m_perm;

 public:
  //!Constructs a permissions object from a user provided os-dependent
  //!permissions.
  permissions(os_permissions_type permissions_type) AUTODDS_NOEXCEPT
  : m_perm(permissions_type)
  {}

  //!Constructs a default permissions object
  permissions() AUTODDS_NOEXCEPT
  { set_default(); }

  //!Sets permissions to default values
  void set_default() AUTODDS_NOEXCEPT
  {
    m_perm = 0644;
  }

  //!Sets permissions to unrestricted access
  void set_unrestricted() AUTODDS_NOEXCEPT
  {
    m_perm = 0666;
  }

  //!Sets permissions from a user provided os-dependent
  void set_permissions(os_permissions_type permissions_type) AUTODDS_NOEXCEPT
  { m_perm = permissions_type; }

  //!Returns stored os-dependent permissions
  os_permissions_type get_permissions() const AUTODDS_NOEXCEPT
  { return m_perm; }

};

}
}
}

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_PERMISSIONS_HPP_