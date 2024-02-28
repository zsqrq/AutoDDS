//
// Created by wz on 24-2-1.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_CREATION_TAGS_HPP_
#define AUTODDS_LIBS_INTERPROCESS_CREATION_TAGS_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"

namespace autodds {
namespace libs {
namespace interprocess {

//!Tag to indicate that the resource must be only created
struct create_only_t {};

//!Tag to indicate that the resource must be only opened
struct open_only_t {};

//!Tag to indicate that the resource must be only opened for reading
struct open_read_only_t {};

//!Tag to indicate that the resource must
//!be only opened privately for reading
struct open_read_private_t {};

//!Tag to indicate that the resource must
//!be only opened for reading
struct open_copy_on_write_t {};

//!Tag to indicate that the resource must
//!be created. If already created, it must be opened.
struct open_or_create_t {};

//!Value to indicate that the resource must
//!be only created
static constexpr create_only_t create_only = create_only_t();

//!Value to indicate that the resource must
//!be only opened
static constexpr open_only_t open_only = open_only_t();

//!Value to indicate that the resource must
//!be only opened for reading
static constexpr open_read_only_t open_read_only = open_read_only_t();

//!Value to indicate that the resource must
//!be created. If already created, it must be opened.
static constexpr open_or_create_t open_or_create = open_or_create_t();

//!Value to indicate that the resource must
//!be only opened for reading
static constexpr open_copy_on_write_t open_copy_on_write = open_copy_on_write_t();

namespace ipcdetail {
enum creat_enum_t { DoCreate, DoOpen, DoOpenOrCreate };
}

}
}
}

#include "libs/interprocess/detail/config_end.hpp"
#endif //AUTODDS_LIBS_INTERPROCESS_CREATION_TAGS_HPP_