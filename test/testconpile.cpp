//
// Created by wz on 24-1-29.
//
#include <iostream>
#include "boost/config/config.hpp"
#include "boost/config/config/compiler/gcc.hpp"
union Data {
  int i;
  float f;
  char c;
};
int main() {

  Data data;
  data.i = 10;
  std::cout << data.i << std::endl;
  std::cout << data.f << std::endl;
  std::cout << sizeof(data) << std::endl;
  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(float) << std::endl;
  data.f = 3.14159;
  std::cout << data.i << std::endl;
  std::cout << data.f << std::endl;
  std::cout << sizeof(data) << std::endl;
  data.c = 'a';
  std::cout << data.i << std::endl;
  std::cout << data.f << std::endl;
  std::cout << data.c << std::endl;
  std::cout << sizeof(data) << std::endl;
  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(float) << std::endl;

  return 0;
}