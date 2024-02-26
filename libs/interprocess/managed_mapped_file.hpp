//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_MANAGED_MAPPED_FILE_HPP_
#define AUTODDS_LIBS_INTERPROCESS_MANAGED_MAPPED_FILE_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/interprocess/detail/managed_open_or_create_impl.hpp"
#include "libs/interprocess/detail/managed_memory_impl.hpp"
#include "libs/interprocess/creation_tags.hpp"
#include "libs/interprocess/detail/file_wrapper.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/file_mapping.hpp"
#include "libs/interprocess/permissions.hpp"

#include "libs/interprocess/mem_algo/rbtree_best_fit.hpp"
#include "libs/interprocess/sync/mutex_family.hpp"
#include "libs/interprocess/indexes/iset_index.hpp"

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

template <
    typename CharType, typename AllocationAlgorithm,
    template <typename IndexConfig> typename IndexType
    >
struct mfile_open_or_create
{
  static const std::size_t segment_maneger_alignment = autodds::libs::move_detail::alignment_of
      <segment_manager<CharType, AllocationAlgorithm, IndexType>>::value;
  static const std::size_t final_segment_manager_alignment = segment_maneger_alignment > AllocationAlgorithm::Alignment ?
      segment_maneger_alignment : AllocationAlgorithm::Alignment;
//  typedef ipcdetail::mana
};

} // namespace ipcdetail
} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_MANAGED_MAPPED_FILE_HPP_