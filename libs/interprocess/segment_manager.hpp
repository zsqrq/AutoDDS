//
// Created by wz on 24-3-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_
#define AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/core/no_exceptions_support.hpp"
#include "libs/interprocess/detail/type_traits.hpp"
#include "libs/interprocess/detail/transform_iterator.hpp"

#include "libs/interprocess/detail/mpl.hpp"
#include "libs/interprocess/detail/nothrow.hpp"
#include "libs/interprocess/detail/segment_manager_helper.hpp"
#include "libs/interprocess/detail/named_proxy.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/offset_ptr.hpp"
#include "libs/interprocess/indexes/iset_index.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/interprocess/allocators/allocator.hpp"
#include "libs/interprocess/smart_ptr/deleter.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/sync/scoped_lock.hpp"

#include "libs/container/detail/minimal_char_traits_header.hpp"
#include "libs/container/detail/placement_new.hpp"
#include "libs/intrusive/detail/minimal_pair_lessequal_chartraits_header.hpp"
#include "libs/common/assert.hpp"

#include <cstddef>
#include <exception>

namespace autodds {
namespace libs {
namespace interprocess {

//!This object is the public base class of segment manager.
//!This class only depends on the memory allocation algorithm
//!and implements all the allocation features not related
//!to named or unique objects.
template<typename MemoryAlgorithm>
class segment_manager_base : private MemoryAlgorithm
{
 public:
  typedef segment_manager_base<MemoryAlgorithm>   segment_manager_base_type;
  typedef
};

} // namespace interprocess
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_SEGMENT_MANAGER_HPP_