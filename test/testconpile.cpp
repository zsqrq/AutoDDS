//
// Created by wz on 24-1-29.
//
#include <iostream>
#include<type_traits>
#include "boost/config.hpp"
#include "boost/config/compiler/gcc.hpp"
#include "boost/interprocess/errors.hpp"
#include "boost/interprocess/exceptions.hpp"
#include "boost/interprocess/creation_tags.hpp"
#include "boost/interprocess/interprocess_fwd.hpp"
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/file_mapping.hpp"

union Data {
  int i;
  float f;
  char c;
};
#     define BOOST_MOVE_HAS_TRIVIAL_COPY(T) (__is_trivially_constructible(T, const T &))
int main() {

//  Data data;
//  data.i = 10;
//  std::cout << data.i << std::endl;
//  std::cout << data.f << std::endl;
//  std::cout << sizeof(data) << std::endl;
//  std::cout << sizeof(int) << std::endl;
//  std::cout << sizeof(float) << std::endl;
//  data.f = 3.14159;
//  std::cout << data.i << std::endl;
//  std::cout << data.f << std::endl;
//  std::cout << sizeof(data) << std::endl;
//  data.c = 'a';
//  std::cout << data.i << std::endl;
//  std::cout << data.f << std::endl;
//  std::cout << data.c << std::endl;
//  std::cout << sizeof(data) << std::endl;
//  std::cout << sizeof(int) << std::endl;
//  std::cout << sizeof(float) << std::endl;
//  boost::interprocess::managed_shared_memory segment(boost::interprocess::create_only,"test", 65536);
  std::cout << __GNUC__ << std::endl;

  return 0;
}