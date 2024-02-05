//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_FILE_FUNCTIONS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_FILE_FUNCTIONS_HPP_
#include "libs/config/config.hpp"
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/errors.hpp"
#include "libs/interprocess/permissions.hpp"
#include "libs/common/static_assert.hpp"

#include <climits>
#include <string>
#include "libs/move/detail/type_traits.hpp"

#  ifdef AUTODDS_HAS_UNISTD_H
#     include <fcntl.h>
#     include <unistd.h>
#     include <sys/types.h>
#     include <sys/stat.h>
#     include <dirent.h>
#     include <cerrno>
#     include <cstdio>
#     if 0
#        include <sys/file.h>
#     endif
#  else
#    error Unknown platform
#  endif
#include <cstring>
#include <cstdlib>

namespace autodds {
namespace libs {
namespace interprocess {

typedef int       file_handle_t;
typedef off_t     offset_t;

typedef struct mapping_handle_impl_t {
  file_handle_t handle;
  bool is_xsi;
} mapping_handle_t;

typedef enum {
  read_only = O_RDONLY,
  read_write = O_RDWR,
  copy_on_write,
  read_private,
  invalid_mode = 0xffff
} mode_t;

typedef enum {
  file_begin = SEEK_SET,
  file_end = SEEK_END,
  file_current = SEEK_CUR
} file_pos_t;

typedef int map_options_t;
static const map_options_t default_map_options = map_options_t(-1);

namespace ipcdetail {

inline mapping_handle_t mapping_handle_from_file_handle(file_handle_t handle) {
  mapping_handle_t ret;
  ret.handle = handle;
  ret.is_xsi = false;
  return ret;
}

inline file_handle_t file_handle_from_mapping_handle(mapping_handle_t handle) {
  return handle.handle;
}

inline bool create_directory(const char *path) {
  ::mode_t m = ::mode_t(0777);
  return ::mkdir(path,m);
}

inline bool open_or_create_shared_directory(const char *path)
{
  const ::mode_t m = ::mode_t(01777);
  const bool created = ::mkdir(path, m) == 0;
  const bool created_or_exists = created || (errno == EEXIST);
  //Try to maximize the chance that the sticky bit is set in shared dirs
  //created with old versions that did not set it (for security reasons)
  const bool chmoded = ::chmod(path, m) == 0;
  return created ? chmoded : created_or_exists;
}

inline bool remove_directory(const char *path)
{  return ::rmdir(path) == 0; }

inline bool get_temporary_path(char* buffer, std::size_t buf_len, std::size_t& required_len) {
  required_len = 5u;
  if (buf_len < required_len) {
    return false;
  } else {
    std::strcpy(buffer, "/tmp");
  }
  return true;
}

inline file_handle_t create_or_open_file
    (const char *name, mode_t mode, const permissions & perm = permissions(), bool temporary = false)
{
  (void)temporary;
  int ret = -1;
  //We need a loop to change permissions correctly using fchmod, since
  //with "O_CREAT only" ::open we don't know if we've created or opened the file.
  while(true){
    ret = ::open(name, ((int)mode) | O_EXCL | O_CREAT, perm.get_permissions());
    if(ret >= 0){
      ::fchmod(ret, perm.get_permissions());
      break;
    }
    else if(errno == EEXIST){
      if((ret = ::open(name, (int)mode)) >= 0 || errno != ENOENT){
        break;
      }
    }
    else{
      break;
    }
  }
  return ret;
}

inline file_handle_t create_new_file
    (const char *name, mode_t mode, const permissions & perm = permissions(), bool temporary = false)
{
  (void)temporary;
  int ret = ::open(name, ((int)mode) | O_EXCL | O_CREAT, perm.get_permissions());
  if(ret >= 0){
    ::fchmod(ret, perm.get_permissions());
  }
  return ret;
}

inline file_handle_t open_existing_file
    (const char* name, mode_t mode, bool temporary = false) {
  (void) temporary;
  return ::open(name, (int)mode);
}

inline bool delete_file(const char* name) {
  return ::unlink(name) == 0;
}

inline bool truncate_file(file_handle_t handle, std::size_t size) {
  typedef autodds::libs::move_detail::make_unsigned<off_t>::type uoff_t;
  AUTODDS_STATIC_ASSERT((sizeof(uoff_t) >= sizeof(std::size_t)));
  if (uoff_t(-1)/2u < uoff_t(size)) {
    errno = EINVAL;
    return false;
  }
  return 0 == ::ftruncate(handle, off_t(size));
}

inline bool get_file_size(file_handle_t handle, offset_t &size)
{
  struct stat data;
  bool ret = 0 == ::fstat(handle, &data);
  if(ret){
    size = data.st_size;
  }
  return ret;
}

inline bool set_file_pointer(file_handle_t handle, offset_t off, file_pos_t pos)
{  return ((off_t)(-1)) != ::lseek(handle, off, (int)pos); }

inline bool get_file_pointer(file_handle_t handle, offset_t &off)
{
  off = ::lseek(handle, 0, SEEK_CUR);
  return off != ((off_t)-1);
}

inline bool write_file(file_handle_t handle, const void *data, std::size_t numdata)
{  return (ssize_t(numdata)) == ::write(handle, data, numdata);  }

inline file_handle_t invalid_file()
{  return -1;  }

inline bool close_file(file_handle_t handle)
{  return ::close(handle) == 0;   }

inline bool acquire_file_lock(file_handle_t handle) {
  struct ::flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  return -1 != ::fcntl(handle, F_SETLKW, &lock);
}

inline bool try_acquire_file_lock(file_handle_t handle, bool &acquired)
{
  struct ::flock lock;
  lock.l_type    = F_WRLCK;
  lock.l_whence  = SEEK_SET;
  lock.l_start   = 0;
  lock.l_len     = 0;
  int ret = ::fcntl(handle, F_SETLK, &lock);
  if(ret == -1){
    return (errno == EAGAIN || errno == EACCES) ?
           (acquired = false, true) : false;
  }
  return (acquired = true);
}

inline bool release_file_lock(file_handle_t handle)
{
  struct ::flock lock;
  lock.l_type    = F_UNLCK;
  lock.l_whence  = SEEK_SET;
  lock.l_start   = 0;
  lock.l_len     = 0;
  return -1 != ::fcntl(handle, F_SETLK, &lock);
}

inline bool acquire_file_lock_sharable(file_handle_t handle) {
  struct ::flock lock;
  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  return -1 != ::fcntl(handle, F_SETLKW,&lock);
}

inline bool try_acquire_file_lock_sharable(file_handle_t handle, bool &acquired)
{
  struct flock lock;
  lock.l_type    = F_RDLCK;
  lock.l_whence  = SEEK_SET;
  lock.l_start   = 0;
  lock.l_len     = 0;
  int ret = ::fcntl(handle, F_SETLK, &lock);
  if(ret == -1){
    return (errno == EAGAIN || errno == EACCES) ?
           (acquired = false, true) : false;
  }
  return (acquired = true);
}

inline bool release_file_lock_sharable(file_handle_t handle)
{  return release_file_lock(handle);   }

#if 0
inline bool acquire_file_lock(file_handle_t hnd)
{  return 0 == ::flock(hnd, LOCK_EX); }

inline bool try_acquire_file_lock(file_handle_t hnd, bool &acquired)
{
   int ret = ::flock(hnd, LOCK_EX | LOCK_NB);
   acquired = ret == 0;
   return (acquired || errno == EWOULDBLOCK);
}

inline bool release_file_lock(file_handle_t hnd)
{  return 0 == ::flock(hnd, LOCK_UN); }

inline bool acquire_file_lock_sharable(file_handle_t hnd)
{  return 0 == ::flock(hnd, LOCK_SH); }

inline bool try_acquire_file_lock_sharable(file_handle_t hnd, bool &acquired)
{
   int ret = ::flock(hnd, LOCK_SH | LOCK_NB);
   acquired = ret == 0;
   return (acquired || errno == EWOULDBLOCK);
}

inline bool release_file_lock_sharable(file_handle_t hnd)
{  return 0 == ::flock(hnd, LOCK_UN); }
#endif

inline bool delete_subdirectories_recursive
    (const std::string &refcstrRootDirectory, const char *dont_delete_this)
{
  DIR *d = opendir(refcstrRootDirectory.c_str());
  if(!d) {
    return false;
  }

  struct dir_close
  {
    DIR *d_;
    dir_close(DIR *d) : d_(d) {}
    ~dir_close() { ::closedir(d_); }
  } dc(d); (void)dc;

  struct ::dirent *de;
  struct ::stat st;
  std::string fn;

  while((de=::readdir(d))) {
    if( de->d_name[0] == '.' && ( de->d_name[1] == '\0'
        || (de->d_name[1] == '.' && de->d_name[2] == '\0' )) ){
      continue;
    }
    if(dont_delete_this && std::strcmp(dont_delete_this, de->d_name) == 0){
      continue;
    }
    fn = refcstrRootDirectory;
    fn += '/';
    fn += de->d_name;

    if(std::remove(fn.c_str())) {
      if(::stat(fn.c_str(), & st)) {
        return false;
      }
      if(S_ISDIR(st.st_mode)) {
        if(!delete_subdirectories_recursive(fn, 0) ){
          return false;
        }
      } else {
        return false;
      }
    }
  }
  return std::remove(refcstrRootDirectory.c_str()) ? false : true;
}

template <class Function>
inline bool for_each_file_in_dir(const char* dir, Function f)
{
  std::string refcstrRootDirectory(dir);
  DIR *d = opendir(refcstrRootDirectory.c_str());
  if (!d) { return false; }

  struct dir_close {
    DIR *d_;
    dir_close(DIR* d) : d_(d){}
    ~dir_close(){ ::closedir(d_); }
  }dc(d);
  (void) dc;
  struct ::dirent *de;
  struct ::stat st;
  std::string fn;

  while ((de = ::readdir(d))) {
    if( de->d_name[0] == '.' && ( de->d_name[1] == '\0'
        || (de->d_name[1] == '.' && de->d_name[2] == '\0' )) ){
      continue;
    }
    fn = refcstrRootDirectory;
    fn += '/';
    fn += de->d_name;

    if (::stat(fn.c_str(), &st)) {
      return false;
    }
    if (!S_ISDIR(st.st_mode)) {
      f(fn.c_str(), de->d_name);
    }
  }
  return true;
}
inline bool delete_subdirectories(const std::string &refcstrRootDirectory, const char *dont_delete_this)
{
  return delete_subdirectories_recursive(refcstrRootDirectory, dont_delete_this );
}

inline std::string get_temporary_path()
{
  std::size_t required_len = 0;
  get_temporary_path((char*)0, 0, required_len);
  std::string ret_str(required_len, char(0));
  get_temporary_path(&ret_str[0], ret_str.size(), required_len);
  while(!ret_str.empty() && !ret_str[ret_str.size()-1]){
    ret_str.erase(ret_str.size()-1);
  }

  return ret_str;
}

#ifdef AUTODDS_INTERPROCESS_WCHAR_NAMED_RESOURCES

inline std::wstring get_temporary_wpath()
{
   std::size_t required_len = 0;
   get_temporary_path((wchar_t*)0, 0, required_len);
   std::wstring ret_str(required_len, char(0));
   get_temporary_path(&ret_str[0], ret_str.size(), required_len);
   while(!ret_str.empty() && !ret_str[ret_str.size()-1]){
      ret_str.erase(ret_str.size()-1);
   }

   return ret_str;
}

#endif
}

}
}
}

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_OS_FILE_FUNCTIONS_HPP_