//
// Created by wz on 24-2-5.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_CHAR_WCHAR_HOLDER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_CHAR_WCHAR_HOLDER_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"

#include <cwchar>
#include <cstring>

namespace autodds {
namespace libs {
namespace interprocess {

class char_wchar_holder
{
 public:
  char_wchar_holder()
      : m_str(), m_is_wide()
  {
    m_str.n = 0;
  }

  char_wchar_holder(const char *nstr)
      : m_str(), m_is_wide()
  {
    m_str.n = new char [std::strlen(nstr)+1];
    std::strcpy(m_str.n, nstr);
  }

  char_wchar_holder(const wchar_t *wstr)
      : m_str(), m_is_wide(true)
  {
    m_str.w = new wchar_t [std::wcslen(wstr)+1];
    std::wcscpy(m_str.w, wstr);
  }

  char_wchar_holder& operator=(const char *nstr)
  {
    char *tmp = new char [std::strlen(nstr)+1];
    this->delete_mem();
    m_str.n = tmp;
    std::strcpy(m_str.n, nstr);
    return *this;
  }

  char_wchar_holder& operator=(const wchar_t *wstr)
  {
    wchar_t *tmp = new wchar_t [std::wcslen(wstr)+1];
    this->delete_mem();
    m_str.w = tmp;
    std::wcscpy(m_str.w, wstr);
    return *this;
  }

  char_wchar_holder& operator=(const char_wchar_holder &other)
  {
    if (other.m_is_wide)
      *this = other.getn();
    else
      *this = other.getw();
    return *this;
  }

  ~char_wchar_holder()
  {
    this->delete_mem();
  }

  wchar_t *getw() const
  {  return m_is_wide ? m_str.w : 0; }

  char *getn() const
  {  return !m_is_wide ? m_str.n : 0; }

  void swap(char_wchar_holder& other)
  {
    char_wchar tmp;
    std::memcpy(&tmp, &m_str, sizeof(char_wchar));
    std::memcpy(&m_str, &other.m_str, sizeof(char_wchar));
    std::memcpy(&other.m_str, &tmp, sizeof(char_wchar));
    //
    bool b_tmp(m_is_wide);
    m_is_wide = other.m_is_wide;
    other.m_is_wide = b_tmp;
  }

 private:

  void delete_mem()
  {
    if(m_is_wide)
      delete [] m_str.w;
    else
      delete [] m_str.n;
  }

  union char_wchar
  {
    char    *n;
    wchar_t *w;
  } m_str;
  bool m_is_wide;
};

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_CHAR_WCHAR_HOLDER_HPP_