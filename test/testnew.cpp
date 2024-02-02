//
// Created by wz on 24-2-1.
//
#include <iostream>
#include <stdio.h>
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

int main() {

#ifdef _GLIBCXX_BEGIN_NAMESPACE_VERSION
  __UINTPTR_TYPE__ a = 1;
  printf("AUTO_GCC = %d\n",a);
#endif
  return 0;
}