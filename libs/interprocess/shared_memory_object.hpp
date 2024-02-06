//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/detail/shared_dir_helpers.hpp"
#include "libs/interprocess/detail/char_wchar_holder.hpp"
#include "libs/interprocess/permissions.hpp"
#include "libs/move/adl_move_swap.hpp"
#include <cstddef>

#if defined(AUTODDS_INTERPROCESS_POSIX_SHARED_MEMORY_OBJECTS)
#  include <string>
#  include <fcntl.h>        //posix_fallocate, O_CREAT, O_*...
#  include <sys/mman.h>     //shm_xxx
#  include <unistd.h>       //ftruncate, close
#  include <sys/stat.h>     //mode_t, S_IRWXG, S_IRWXO, S_IRWXU,
#  if defined(AUTODDS_INTERPROCESS_RUNTIME_FILESYSTEM_BASED_POSIX_SHARED_MEMORY)
#     if defined(__FreeBSD__)
#        include <sys/sysctl.h>
#     endif
#  endif
#else
//
#endif

namespace autodds {
namespace libs {
namespace interprocess {

//!A class that wraps a shared memory mapping that can be used to
//!create mapped regions from the mapped files

class shared_memory_object
{
  //Non-copyable and non-assignable
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(shared_memory_object)
 public:

  shared_memory_object() AUTODDS_NOEXCEPT;

  //!Creates a shared memory object with name "name" and mode "mode", with the access mode "mode"
  //!If the file previously exists, throws an error.*/
  shared_memory_object(create_only_t, const char* name, mode_t mode, const permissions &perm = permissions())
  { this->priv_open_or_create(ipcdetail::DoOpenOrCreate, name, mode, perm);}

#if defined(AUTODDS_INTERPROCESS_WCHAR_NAMED_RESOURCES)
  shared_memory_object(create_only_t, const wchar_t*name, mode_t mode, const permissions &perm = permissions())
   {  this->priv_open_or_create(ipcdetail::DoCreate, name, mode, perm);  }

   shared_memory_object(open_or_create_t, const wchar_t*name, mode_t mode, const permissions &perm = permissions())
   {  this->priv_open_or_create(ipcdetail::DoOpenOrCreate, name, mode, perm);  }

   shared_memory_object(open_only_t, const wchar_t*name, mode_t mode)
   {  this->priv_open_or_create(ipcdetail::DoOpen, name, mode, permissions());  }
#endif

  //!Moves the ownership of "moved"'s shared memory object to *this.
  //!After the call, "moved" does not represent any shared memory object.
  //!Does not throw
  shared_memory_object(AUTODDS_RV_REF(shared_memory_object) moved) AUTODDS_NOEXCEPT
      : m_handle(file_handle_t(ipcdetail::invalid_file())), m_mode(read_only)
  { this->swap(moved); }

  //!Moves the ownership of "moved"'s shared memory to *this.
  //!After the call, "moved" does not represent any shared memory.
  //!Does not throw
  shared_memory_object& operator=( AUTODDS_RV_REF(shared_memory_object) moved) AUTODDS_NOEXCEPT
  {
    shared_memory_object tmp(autodds::libs::move(moved));
    this->swap(tmp);
    return *this;
  }

  //!Swaps the shared_memory_objects. Does not throw
  void swap(shared_memory_object &moved) AUTODDS_NOEXCEPT;

  //!Erases a shared memory object from the system.
  //!Returns false on error. Never throws
  static bool remove(const char* name);

#if defined(AUTODDS_INTERPROCESS_WCHAR_NAMED_RESOURCES)
  static bool remove(const wchar_t *name);
#endif

  //!Sets the size of the shared memory mapping
  void truncate(offset_t length);

  //!Destroys *this and indicates that the calling process is finished using
  //!the resource. All mapped regions are still
  //!valid after destruction. The destructor function will deallocate
  //!any system resources allocated by the system for use by this process for
  //!this resource. The resource can still be opened again calling
  //!the open constructor overload. To erase the resource from the system
  //!use remove().
  ~shared_memory_object();

  //!Returns the name of the shared memory object.
  const char* get_name() const AUTODDS_NOEXCEPT;

  //!Returns true if the size of the shared memory object
  //!can be obtained and writes the size in the passed reference
  bool get_size(offset_t& size) const AUTODDS_NOEXCEPT;

  //!Returns access mode
  mode_t get_mode() const AUTODDS_NOEXCEPT;

  //!Returns mapping handle. Never throws.
  mapping_handle_t get_mapping_handle() const AUTODDS_NOEXCEPT;

 private:

  //!Closes a previously opened file mapping. Never throws.
  void priv_close();

  //!Opens or creates a shared memory object.
  template <class CharT>
  bool priv_open_or_create(ipcdetail::creat_enum_t type, const CharT* filename, mode_t mode, const permissions& perm);

  file_handle_t m_handle;
  mode_t m_mode;
  char_wchar_holder m_filename;
};

inline shared_memory_object::shared_memory_object() AUTODDS_NOEXCEPT
    : m_handle(file_handle_t(ipcdetail::invalid_file())), m_mode(read_only)
{}

inline shared_memory_object::~shared_memory_object()
{ this->priv_close(); }

inline const char* shared_memory_object::get_name() const AUTODDS_NOEXCEPT
{ return m_filename.getn(); }

inline bool shared_memory_object::get_size(offset_t &size) const AUTODDS_NOEXCEPT
{ return ipcdetail::get_file_size((file_handle_t)m_handle, size); }

inline void shared_memory_object::swap(shared_memory_object &other) AUTODDS_NOEXCEPT
{
  autodds::libs::adl_move_swap(m_handle, other.m_handle);
  autodds::libs::adl_move_swap(m_mode, other.m_mode);
  m_filename.swap(other.m_filename);
}

inline mapping_handle_t shared_memory_object::get_mapping_handle() const AUTODDS_NOEXCEPT
{
  return ipcdetail::mapping_handle_from_file_handle(m_handle);
}

inline mode_t shared_memory_object::get_mode() const AUTODDS_NOEXCEPT
{ return m_mode; }

template <class CharT>
inline bool shared_memory_object::priv_open_or_create(ipcdetail::creat_enum_t type,
                                                      const CharT *filename,
                                                      autodds::libs::interprocess::mode_t mode,
                                                      const autodds::libs::interprocess::permissions &perm)
{
#if defined(AUTODDS_INTERPROCESS_FILESYSTEM_BASED_POSIX_SHARED_MEMORY)
  const bool add_leading_slash = false;
#else
  const bool add_leading_slash = true;
#endif
  std::basic_string<CharT> fname;
  if (add_leading_slash) {
    autodds::libs::ipcdetail::add_leading_slash(filename, fname);
  } else {
    ::autodds::libs::ipcdetail::create_shared_dir_cleaning_old_and_get_filepath(filename,fname);
  }

  // Create new mapping
  int oflag = 0;
  if (mode == read_only) {
    oflag |= O_RDONLY;
  }
  else if (mode == read_write) {
    oflag |= O_RDWR;
  }
  else {
    error_info err(mode_error);
    throw interprocess_exception(err);
  }
  ::mode_t unix_perm = perm.get_permissions();

  switch (type) {
    case ipcdetail::DoOpen:
    {
      //No oflag addition
      m_handle = shm_open(fname.c_str(), oflag, unix_perm);
    }
      break;
    case ipcdetail::DoOpenOrCreate:
    {
      //We need a create/open loop to change permissions correctly using fchmod, since
      //with "O_CREAT" only we don't know if we've created or opened the shm.
      while (true)
      {
        // Try to create shared memory
        m_handle = shm_open(fname.c_str(), oflag | (O_CREAT | O_EXCL), unix_perm);
        // If successful, change real pemission
        if (m_handle >= 0)
        {
          ::fchmod(m_handle, unix_perm);
        }
        // If already exits, try to open
        else if (errno == EEXIST)
        {
          m_handle = shm_open(fname.c_str(), oflag, unix_perm);
          //If open fails and errno tells the file does not exist
          //(shm was removed between creation and opening tries), just retry
          if (m_handle < 0 && errno ==ENOENT)
          {
            continue;
          }
        }
        break;
      }
    }
      break;
    default:
    {
      error_info err = other_error;
      throw interprocess_exception(err);
    }
  }
  if (m_handle < 0) {
    error_info err = errno;
    this->priv_close();
    throw interprocess_exception(err);
  }
  m_filename = filename;
  m_mode = mode;
  return true;
}

inline bool shared_memory_object::remove(const char *filename)
{
  AUTODDS_TRY{
      std::string filepath;
#if defined(AUTODDS_INTERPROCESS_FILESYSTEM_BASED_POSIX_SHARED_MEMORY)
      const bool add_leading_slash = false;
#elif defined(AUTODDS_INTERPROCESS_RUNTIME_FILESYSTEM_BASED_POSIX_SHARED_MEMORY)
      const bool add_leading_slash = !shared_memory_object_detail::use_filesystem_based_posix();
#else
      const bool add_leading_slash = true;
#endif
      if(add_leading_slash){
        autodds::libs::ipcdetail::add_leading_slash(filename, filepath);
      }
      else{
        autodds::libs::ipcdetail::shared_filepath(filename, filepath);
      }
      return 0 == shm_unlink(filepath.c_str());
  }
  AUTODDS_CATCH(...){
    return false;
  } AUTODDS_CATCH_END
}

inline void shared_memory_object::truncate(autodds::libs::interprocess::offset_t length)
{
#ifdef AUTODDS_INTERPROCESS_POSIX_FALLOCATE
  int ret = EINTR;
   while (EINTR == ret) {
      ret = posix_fallocate(m_handle, 0, length);
   }

   if (ret && ret != EOPNOTSUPP && ret != ENODEV){
      error_info err(ret);
      throw interprocess_exception(err);
   }
   //ftruncate fallback
#endif //AUTODDS_INTERPROCESS_POSIX_FALLOCATE

  handle_eintr:
  if (0 != ftruncate(m_handle, length)){
    if (errno == EINTR)
      goto handle_eintr;
    error_info err(system_error_code());
    throw interprocess_exception(err);
  }
}

inline void shared_memory_object::priv_close()
{
  if (m_handle != -1)
  {
    ::close(m_handle);
    m_handle = -1;
  }
}

//!A class that stores the name of a shared memory
//!and calls shared_memory_object::remove(name) in its destructor
//!Useful to remove temporary shared memory objects in the presence
//!of exceptions
class remove_shared_memory_on_destroy
{
  const char* m_name;
 public:
  remove_shared_memory_on_destroy(const char* name)
  : m_name(name)
  {}

  ~remove_shared_memory_on_destroy()
  {
    shared_memory_object::remove(m_name);
  }
};

}
}
}

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_SHARED_MEMORY_OBJECT_HPP_