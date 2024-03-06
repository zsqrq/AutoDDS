//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_ALGO_TYPE_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_ALGO_TYPE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {

enum algo_types {
  CircularListAlgorithms,
  CircularSListAlgorithms,
  LinearSListAlgorithms,
  CommonSListAlgorithms,
  BsTreeAlgorithms,
  RbTreeAlgorithms,
  AvlTreeAlgorithms,
  SgTreeAlgorithms,
  SplayTreeAlgorithms,
  TreapAlgorithms,
  UnorderedAlgorithms,
  UnorderedCircularSlistAlgorithms,
  AnyAlgorithm
};

template<algo_types AlgoType, typename NodeTraits>
struct get_algo;

template<algo_types AlgoType, typename ValueTraits, typename NodePtrCompare, typename ExtraChecker>
struct get_node_checker;

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_ALGO_TYPE_HPP_