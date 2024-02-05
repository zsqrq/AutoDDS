//
// Created by wz on 24-2-1.
//
#include <iostream>
#include <stdio.h>
#include <type_traits>
# include <limits.h>
#include <stdint-gcc.h>
#include <stdint.h>
#include "libs/config/config.hpp"
//#include "libs/config/compiler/gcc.hpp"
#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/errors.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/interprocess/permissions.hpp"
#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/common/cstdint.hpp"
#include "libs/interprocess/shared_memory_object.hpp"
#include "libs/move/detail/type_traits.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/interprocess/detail/shared_dir_helpers.hpp"

template<class T>
struct add_lvalue_reference
{  typedef T& type;  };

template<class T> struct add_lvalue_reference<T&>                 {  typedef T& type;  };
template<>        struct add_lvalue_reference<void>               {  typedef void type;   };
template<>        struct add_lvalue_reference<const void>         {  typedef const void type;  };
template<>        struct add_lvalue_reference<volatile void>      {  typedef volatile void type;   };
template<>        struct add_lvalue_reference<const volatile void>{  typedef const volatile void type;   };

template <typename T>
struct unvoid_ref : add_lvalue_reference<T> {};

template <>
struct unvoid_ref<void> { typedef unvoid_ref & type; };

template <>
struct unvoid_ref<void (&)()> { typedef unvoid_ref & type; };

template <>
struct unvoid_ref<const void> { typedef unvoid_ref & type; };

template <>
struct unvoid_ref<volatile void> { typedef unvoid_ref & type; };

template <>
struct unvoid_ref<const volatile void> { typedef unvoid_ref & type; };

// 一个泛型的函数包装器
template <typename Func, typename... Args>
auto call_with_log(Func func, Args&&... args) {
  // 使用 decltype 检查函数返回类型
  if constexpr (std::is_same_v<decltype(func(std::forward<Args>(args)...)), void>) {
    std::cout << "Calling function..." << std::endl;
    func(std::forward<Args>(args)...); // 直接调用，不尝试赋值
    std::cout << "Function called." << std::endl;
  } else {
    std::cout << "Calling function..." << std::endl;
    auto result = func(std::forward<Args>(args)...); // 调用函数并保存返回值
    std::cout << "Function called." << std::endl;
    return result; // 返回结果
  }
}

template <typename Func, typename... Args>
auto Call_with_log(Func func, Args&&... args) -> typename unvoid_ref<decltype(func(std::forward<Args>(args)...))>::type {
  std::cout << "Calling function..." << std::endl;
  auto result = func(std::forward<Args>(args)...); // 调用函数
  std::cout << "Function called." << std::endl;
  return result;
}

// 用于测试的函数
void voidFunction() {
  std::cout << "Inside voidFunction" << std::endl;
}

int intFunction() {
  std::cout << "Inside intFunction" << std::endl;
  return 42;
}


int main() {
  AUTODDS_STATIC_ASSERT(true);
#ifdef _GLIBCXX_BEGIN_NAMESPACE_VERSION
  __UINTPTR_TYPE__ a = 1;
  auto s = std::is_copy_assignable<int>::value;
//  s = std::is_em
  printf("AUTO_GCC = %d\n",std::is_trivially_assignable_v<decltype(a),int>);
  printf("AUTO_GCC = %d\n",a);
#endif
  std::string test = "/home/wz/docker/AutoDDS/testremove";
  DIR *d = opendir(test.c_str());
  struct dirent *de;
  struct ::stat st;
  std::string fn;
  while((de=::readdir(d))) {
    if( de->d_name[0] == '.' && ( de->d_name[1] == '\0'
        || (de->d_name[1] == '.' && de->d_name[2] == '\0' )) ){
      continue;
    }
    std::cout <<de->d_name  << std::endl;
    fn = test;
    fn += '/';
    fn += de->d_name;
    std::cout << fn << std::endl;
    if(std::remove(fn.c_str())) {
      if(::stat(fn.c_str(), & st)) {
        return false;
      }

    }
  }
  std::remove(test.c_str());

//  Call_with_log(voidFunction); // 调用返回 void 的函数
//  int result = call_with_log(intFunction); // 调用返回 int 的函数
//  std::cout << "Result of intFunction: " << result << std::endl;
  return 0;
}