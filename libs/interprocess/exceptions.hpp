//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_EXCEPTIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_EXCEPTIONS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#include "libs/config/config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/errors.hpp"
#include <stdexcept>

namespace autodds {
namespace libs {
namespace interprocess {

//!This class is the base class of all exceptions
//!thrown by autodds::libs::interprocess

class AUTODDS_SYMBOL_VISIBLE interprocess_exception : public std::exception {
 public:
  interprocess_exception(const char* err) AUTODDS_NOEXCEPT
  : m_err(other_error)
  {
    AUTODDS_TRY   {  m_str = err; }
    AUTODDS_CATCH(...) {} AUTODDS_CATCH_END
  }

  interprocess_exception(const error_info& error_info, const char* str = 0)
  : m_err(error_info)
  {
    AUTODDS_TRY{
        if (m_err.get_native_error() != 0) {
          fill_system_message(m_err.get_native_error(), m_str);
        }
        else if (str) {
          m_str = str;
        }
        else {
          m_str = "autodds::libs::interprocess_exception::library_error";
        }
    }
    AUTODDS_CATCH(...){} AUTODDS_CATCH_END
  }

  ~interprocess_exception() AUTODDS_NOEXCEPT_OR_NOTHROW AUTODDS_OVERRIDE {}

  const char* what() const AUTODDS_NOEXCEPT_OR_NOTHROW AUTODDS_OVERRIDE
  { return m_str.c_str(); }

  native_error_t get_native_error() const AUTODDS_NOEXCEPT
  { return m_err.get_native_error(); }

  error_code_t get_error_code() const AUTODDS_NOEXCEPT
  { return m_err.get_error_code(); }

 private:
  error_info m_err;
  std::string m_str;
};

//!This is the exception thrown by shared interprocess_mutex family when a deadlock situation
//!is detected or when using a interprocess_condition the interprocess_mutex is not locked
class AUTODDS_SYMBOL_VISIBLE lock_exception

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_EXCEPTIONS_HPP_