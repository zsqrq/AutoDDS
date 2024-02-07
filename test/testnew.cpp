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
//#include "libs/interprocess/detail/shared_dir_helpers.hpp"
#include "libs/interprocess/mapped_region.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/pointer_traits.hpp"

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

struct WithValueType {
  typedef int value_type;
};

struct WithoutValueType {
  // 没有 value_type 定义
};
AUTODDS_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(value_type)

struct MyContainer {
  // 假设这个容器没有定义value_type
};

struct DefaultValueType {
  typedef double type; // 如果 T 中没有定义 value_type，则使用 double
};
// 检测一个类型T是否有名为value_type的嵌套类型
bool hasValueType = AUTODDS_INTRUSIVE_HAS_TYPE( , MyContainer, value_type);
// 获取类型T的value_type，如果不存在，则使用int作为默认类型
typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT(, MyContainer, value_type, int) ContainerValueType;

AUTODDS_INTRUSIVE_INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(value_type)
typedef autodds_intrusive_eval_default_type_value_type<MyContainer, DefaultValueType>::type ContainerValueType2;

typedef AUTODDS_INTRUSIVE_OBTAIN_TYPE_WITH_EVAL_DEFAULT(,MyContainer,value_type,DefaultValueType) ContainerValueType3;

AUTODDS_INTRUSIVE_INTERNAL_STATIC_BOOL_IS_TRUE(is_special_traits, is_special)

struct SpecialType {
  constexpr static bool is_special = true;
};

struct NormalType {};

bool isSpecialType = is_special_traits_bool_is_true<SpecialType>::value; // true
bool isNormalType = is_special_traits_bool_is_true<NormalType>::value; // false

AUTODDS_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(check_for_my_static_func, my_static_func)
struct MyClass {
  static void my_static_func() {}
};

//
struct MyOtherClass {};

AUTODDS_INTRUSIVE_HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(has_doSomething, doSomething)
class MyClass1 {
 public:
  void doSomething(int value) { /* 实现 */ }
};

class AnotherClass {
  // 没有 doSomething 成员函数
};

bool hasDoSomethingMyClass = has_doSomething<MyClass1, void>::value; // 将会是 true
bool hasDoSomethingAnotherClass = has_doSomething<AnotherClass,void>::value; // 将会是 false

int main() {
  AUTODDS_STATIC_ASSERT(true);
#ifdef _GLIBCXX_BEGIN_NAMESPACE_VERSION
  __UINTPTR_TYPE__ a = 1;
  auto s = std::is_copy_assignable<int>::value;
//  s = std::is_em
  printf("AUTO_GCC = %d\n",std::is_trivially_assignable_v<decltype(a),int>);
  printf("AUTO_GCC = %d\n",a);
#endif
//  std::string test = "/home/wz/docker/AutoDDS/testremove";
//  DIR *d = opendir(test.c_str());
//  struct dirent *de;
//  struct ::stat st;
//  std::string fn;
//  while((de=::readdir(d))) {
//    if( de->d_name[0] == '.' && ( de->d_name[1] == '\0'
//        || (de->d_name[1] == '.' && de->d_name[2] == '\0' )) ){
//      continue;
//    }
//    std::cout <<de->d_name  << std::endl;
//    fn = test;
//    fn += '/';
//    fn += de->d_name;
//    std::cout << fn << std::endl;
//    if(std::remove(fn.c_str())) {
//      if(::stat(fn.c_str(), & st)) {
//        return false;
//      }
//
//    }
//  }
//  std::remove(test.c_str());

  std::cout << "WithValueType has value_type: "
            << std::is_same<autodds_intrusive_default_type_value_type<WithValueType, double>::type, int>::value
            << std::endl;

  // 检查 WithoutValueType
  std::cout << "WithoutValueType has value_type: "
            << std::is_same<autodds_intrusive_default_type_value_type<WithoutValueType, double>::type, double>::value
            << std::endl;

  std::cout << "bool has value_type: "<< hasDoSomethingMyClass <<std::endl;

//  提问("大剑哥有多帅");
//  回答;


//  Call_with_log(voidFunction); // 调用返回 void 的函数
//  int result = call_with_log(intFunction); // 调用返回 int 的函数
//  std::cout << "Result of intFunction: " << result << std::endl;
  return 0;
}