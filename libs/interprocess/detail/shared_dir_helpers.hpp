//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_SHARED_DIR_HELPERS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_SHARED_DIR_HELPERS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/errors.hpp"
#include "libs/interprocess/exceptions.hpp"
#include <string>

namespace autodds {
namespace libs {
namespace ipcdetail {
using namespace autodds::libs::interprocess;

template <class CharT>
struct shared_dir_constants;

template<>
struct shared_dir_constants<char>
{
  static char dir_separator()
  {  return '/';   }

  static const char *dir_interprocess()
  {  return "/autodds_interprocess";   }
};

template<>
struct shared_dir_constants<wchar_t>
{
  static wchar_t dir_separator()
  {  return L'/';   }

  static const wchar_t *dir_interprocess()
  {  return L"/autodds_interprocess";   }
};

template <class CharT>
inline void get_shared_dir_root(std::basic_string<CharT>& dir_path) {
  dir_path = "/tmp";
  if (dir_path.empty()) {
    error_info err = system_error_code();
    throw interprocess_exception(err);
  }
  dir_path += shared_dir_constants<CharT>::dir_interprocess();
}

#if defined(AUTODDS_INTERPROCESS_SHARED_DIR_FUNC) && defined(AUTODDS_INTERPROCESS_SHARED_DIR_PATH)
#error "Error: Both AUTODDS_INTERPROCESS_SHARED_DIR_FUNC and AUTODDS_INTERPROCESS_SHARED_DIR_PATH defined!"
#endif

#ifdef AUTODDS_INTERPROCESS_SHARED_DIR_FUNC

// When AUTODDS_INTERPROCESS_SHARED_DIR_FUNC is defined, users have to implement
   // get_shared_dir
   void get_shared_dir(std::string &shared_dir);

   // When AUTODDS_INTERPROCESS_SHARED_DIR_FUNC is defined, users have to implement
   // get_shared_dir
   void get_shared_dir(std::wstring &shared_dir);

#else

#if defined(AUTODDS_INTERPROCESS_SHARED_DIR_PATH)
inline void get_shared_dir(std::string &shared_dir)
{
   shared_dir = AUTODDS_INTERPROCESS_SHARED_DIR_PATH;
}

#endif

template<class CharT>
inline void get_shared_dir(std::basic_string<CharT> &shared_dir)
{
  get_shared_dir_root(shared_dir);
#if defined(AUTODDS_INTERPROCESS_HAS_KERNEL_BOOTTIME)
  shared_dir += shared_dir_constants<CharT>::dir_separator();
         get_bootstamp(shared_dir, true);
#endif
}
#endif

template<class CharT>
inline void shared_filepath(const CharT *filename, std::basic_string<CharT> &filepath)
{
  get_shared_dir(filepath);
  filepath += shared_dir_constants<CharT>::dir_separator();
  filepath += filename;
}

template<class CharT>
inline void create_shared_dir_and_clean_old(std::basic_string<CharT> &shared_dir)
{
#if defined(AUTODDS_INTERPROCESS_SHARED_DIR_PATH) || defined(AUTODDS_INTERPROCESS_SHARED_DIR_FUNC)
  get_shared_dir(shared_dir);
#else
  //First get the temp directory
  std::basic_string<CharT> root_shared_dir;
  get_shared_dir_root(root_shared_dir);

  //If fails, check that it's because already exists
  if(!open_or_create_shared_directory(root_shared_dir.c_str())){
    error_info info(system_error_code());
    throw interprocess_exception(info);
  }

#if defined(AUTODDS_INTERPROCESS_HAS_KERNEL_BOOTTIME)
  get_shared_dir(shared_dir);

         //If fails, check that it's because already exists
         if(!open_or_create_shared_directory(shared_dir.c_str())){
            error_info info(system_error_code());
            throw interprocess_exception(info);
         }
         //Now erase all old directories created in the previous boot sessions
         std::basic_string<CharT> subdir = shared_dir;
         subdir.erase(0, root_shared_dir.size()+1);
         delete_subdirectories(root_shared_dir, subdir.c_str());
#else
  shared_dir = root_shared_dir;
#endif
#endif
}

template<class CharT>
inline void create_shared_dir_cleaning_old_and_get_filepath(const CharT *filename, std::basic_string<CharT> &shared_dir)
{
  create_shared_dir_and_clean_old(shared_dir);
  shared_dir += shared_dir_constants<CharT>::dir_separator();
  shared_dir += filename;
}

template<class CharT>
inline void add_leading_slash(const CharT *name, std::basic_string<CharT> &new_name)
{
  if(name[0] != shared_dir_constants<CharT>::dir_separator()){
    new_name = shared_dir_constants<CharT>::dir_separator();
  }
  new_name += name;
}

}
}
}

#include "libs/interprocess/detail/config_end.hpp"

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_SHARED_DIR_HELPERS_HPP_