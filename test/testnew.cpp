//
// Created by wz on 24-2-1.
//
#include <iostream>
#include <stdio.h>
# include <limits.h>
#include "libs/config/config.hpp"
//#include "libs/config/compiler/gcc.hpp"
#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/errors.hpp"

int main() {

#ifdef __GNUC__
  printf("AUTO_GCC = %d\n",_XOPEN_UNIX);
#endif
  return 0;
}