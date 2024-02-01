//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_CONFIG_DETAIL_COMPILER_SELECTION_CONFIG_HPP_
#define AUTODDS_LIBS_CONFIG_DETAIL_COMPILER_SELECTION_CONFIG_HPP_

#if defined __CUDACC__
//  NVIDIA CUDA C++ compiler for GPU
#   include "libs/config/compiler/nvcc.hpp"

#endif

#if defined(__GNUC__) && !defined(__ibmxl)
#define AUTODDS_COMPILER_CONFIG  "libs/config/compiler/gcc.hpp"
#endif

#endif //AUTODDS_LIBS_CONFIG_DETAIL_COMPILER_SELECTION_CONFIG_HPP_