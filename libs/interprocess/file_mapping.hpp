//
// Created by wz on 24-2-9.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_FILE_MAPPING_HPP_
#define AUTODDS_LIBS_INTERPROCESS_FILE_MAPPING_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"#in
#include "libs/interprocess/detail/workaround.hpp"

#if !defined(AUTODDS_INTERPROCESS_MAPPED_FILES)
#error "Autodds .Interprocess: This platform does not support memory mapped files!"
#endif

#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/detail/char_wchar_holder.hpp"
#include "libs/move/utility_core.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

//!A class that wraps a file-mapping that can be used to
//!create mapped regions from the mapped files
class file_mapping
{
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(file_mapping);
 public:

  file_mapping() AUTODDS_NOEXCEPT;

  // Opens a file mapping of file "filename", starting in offset
  // "file_offset", and the mapping's size will be "size". The mapping
  // can be opened for read-only "read_only" or read-write "read_write"
  // modes. Throws interprocess_exception on error.
  file_mapping(const char* filename, mode_t mode);

#if defined(AUTODDS_INTERPROCESS_WCHAR_NAMED_RESOURCES)
  file_mapping(const wchar_t *filename, mode_t mode);
#endif

  // Move Constructor
  file_mapping(AUTODDS_RV_REF(file_mapping) moved) AUTODDS_NOEXCEPT
      : m_handle(file_handle_t(ipcdetail::invalid_file())), m_mode(read_only)
  { this->swap(moved); }

  file_mapping& operator = ( AUTODDS_RV_REF(file_mapping) moved) AUTODDS_NOEXCEPT
  {
    file_mapping tmp(autodds::libs::move(moved));
    this->swap(tmp);
    return *this;
  }

  void swap(file_mapping& other) AUTODDS_NOEXCEPT;

  mode_t get_mode() const AUTODDS_NOEXCEPT;

  mapping_handle_t get_mapping_handle() const AUTODDS_NOEXCEPT;

  ~file_mapping();

  const char* get_name() const AUTODDS_NOEXCEPT;

  // Removes the file named "filename" even if it's been memory mapped.
  static bool remove(const char* filename);

#if defined(AUTODDS_INTERPROCESS_WCHAR_NAMED_RESOURCES)
  static bool remove(const wchar_t *filename);
#endif
 private:

  void priv_close();
  file_handle_t         m_handle;
  mode_t                m_mode;
  char_wchar_holder     m_filename;
};

inline file_mapping::file_mapping() AUTODDS_NOEXCEPT
    : m_handle(file_handle_t(ipcdetail::invalid_file())), m_mode(read_only)
{}

inline file_mapping::~file_mapping()
{ this->priv_close(); }

inline const char* file_mapping::get_name() const AUTODDS_NOEXCEPT
{ return m_filename.getn(); }

inline void file_mapping::swap(file_mapping &other) AUTODDS_NOEXCEPT
{
  simple_swap(m_handle, other.m_handle);
  simple_swap(m_mode, other.m_mode);
  m_filename.swap(other.m_filename);
}

inline mapping_handle_t file_mapping::get_mapping_handle() const AUTODDS_NOEXCEPT
{ return ipcdetail::mapping_handle_from_file_handle(m_handle); }

inline mode_t file_mapping::get_mode() const AUTODDS_NOEXCEPT
{ return m_mode; }

inline file_mapping::file_mapping(const char *filename, mode_t mode)
    : m_filename(filename)
{
  if (mode != read_write && mode != read_only) {
    error_info err = other_error;
    throw interprocess_exception(err);
  }
  // Open file
  m_handle = ipcdetail::open_existing_file(filename, mode);

  // Check for eror
  if (m_handle == ipcdetail::invalid_file()) {
    error_info err = system_error_code();
    this->priv_close();
    throw interprocess_exception(err);
  }
  m_mode = mode;
}

inline bool file_mapping::remove(const char *filename)
{ return ipcdetail::delete_file(filename); }

inline void file_mapping::priv_close()
{
  if (m_handle != ipcdetail::invalid_file()) {
    ipcdetail::close_file(m_handle);
    m_handle = ipcdetail::invalid_file();
  }
}

class remove_file_on_destroy
{
  const char * m_name;
 public:
  remove_file_on_destroy(const char* name)
      : m_name(name)
  {}

  ~remove_file_on_destroy()
  { ipcdetail::delete_file(m_name); }
};

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_FILE_MAPPING_HPP_