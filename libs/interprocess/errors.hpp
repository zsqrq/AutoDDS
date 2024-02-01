//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_ERRORS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_ERRORS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif

#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#pragma once
#endif
#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include <string>
#  ifdef AUTODDS_HAS_UNISTD_H
#    include <cerrno>         //Errors
#    include <cstring>        //strerror
#  else  //ifdef AUTODDS_HAS_UNISTD_H
#    error Unknown platform
#  endif //ifdef AUTODDS_HAS_UNISTD_H

namespace autodds{
namespace libs {
namespace interprocess {

inline int system_error_code() {
  return errno;
}

inline void fill_system_message(int sys_err, std::string& str) {
  str = std::strerror(sys_err);
}

enum error_code_t {
  no_error = 0,
  system_error,     // system generated error; if possible, is translated
  // to one of the more specific errors below.
  other_error,      // library generated error
  security_error,   // includes access rights, permissions failures
  read_only_error,
  io_error,
  path_error,
  not_found_error,
  busy_error,       // implies trying again might succeed
  already_exists_error,
  not_empty_error,
  is_directory_error,
  out_of_space_error,
  out_of_memory_error,
  out_of_resource_error,
  lock_error,
  sem_error,
  mode_error,
  size_error,
  corrupted_error,
  not_such_file_or_directory,
  invalid_argument,
  timeout_when_locking_error,
  timeout_when_waiting_error,
  owner_dead_error,
  not_recoverable
};
#if (__cplusplus >= 201103L)
using native_error_t = int;
#else
typedef int native_error_t;
#endif

struct ec_xlate
{
  native_error_t sys_err_code;
  error_code_t   err_code;
};

static const ec_xlate err_code_table[] = {
    { EACCES, security_error },
    { EROFS, read_only_error },
    { EIO, io_error },
    { ENAMETOOLONG, path_error },
    { ENOENT, not_found_error },
//    { ENOTDIR, not_directory_error },
    { EAGAIN, busy_error },
    { EBUSY, busy_error },
    { ETXTBSY, busy_error },
    { EEXIST, already_exists_error },
    { ENOTEMPTY, not_empty_error },
    { EISDIR, is_directory_error },
    { ENOSPC, out_of_space_error },
    { ENOMEM, out_of_memory_error },
    { EMFILE, out_of_resource_error },
    { ENOENT, not_such_file_or_directory },
    { EINVAL, invalid_argument }
};

inline error_code_t lookup_error(native_error_t err) {
  const ec_xlate *cur = &err_code_table[0];
  const ec_xlate *end = cur + sizeof(err_code_table) / sizeof(ec_xlate);

  for (;cur != end; ++cur) {
    if (err == cur->sys_err_code) return cur->err_code;
  }
  return system_error;
}

struct error_info {
  error_info(error_code_t ec = other_error)
  : m_nat(0), m_ec(ec)
  {}

  error_info(native_error_t sys_err_code)
  : m_nat(sys_err_code), m_ec(lookup_error(sys_err_code))
  {}

  error_info& operator = (error_code_t ec) {
    m_nat = 0;
    m_ec = ec;
    return *this;
  }

  error_info& operator = (native_error_t sys_err_code) {
    m_nat = sys_err_code;
    m_ec = lookup_error(sys_err_code);
    return *this;
  }

  native_error_t get_native_error() const
  { return m_nat; }

  error_code_t get_error_code() const
  { return m_ec; }

 private:
  native_error_t m_nat;
  error_code_t m_ec;
};

}
}
}
#include "libs/interprocess/detail/config_end.hpp"
#endif //AUTODDS_LIBS_INTERPROCESS_ERRORS_HPP_