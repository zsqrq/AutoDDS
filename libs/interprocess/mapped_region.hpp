//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_MAPPED_REGION_HPP_
#define AUTODDS_LIBS_INTERPROCESS_MAPPED_REGION_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"

#include "libs/interprocess/interprocess_fwd.hpp"
#include "libs/interprocess/exceptions.hpp"
#include "libs/move/utility_core.hpp"
#include "libs/interprocess/detail/utilities.hpp"
#include "libs/interprocess/detail/os_file_functions.hpp"
#include "libs/common/cstdint.hpp"
#include "libs/common/assert.hpp"
#include "libs/move/adl_move_swap.hpp"
#include <string>

#  ifdef AUTODDS_HAS_UNISTD_H
#    include <fcntl.h>
#    include <sys/mman.h>     //mmap
#    include <unistd.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#    if defined(AUTODDS_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)
#      include <sys/shm.h>      //System V shared memory...
#    endif
#    include "libs/common/assert.hpp"
#  else
#    error Unknown platform
#  endif

namespace autodds {
namespace libs {
namespace interprocess {

namespace ipcdetail
{
class interprocess_tester;
class raw_mapped_region_creator;
}

//!The mapped_region class represents a portion or region created from a
//!memory_mappable object.
//!The OS can map a region bigger than the requested one, as region must
//!be multiple of the page size, but mapped_region will always refer to
//!the region specified by the user.
class mapped_region
{
  // non copyble
 AUTODDS_MOVABLE_BUT_NOT_COPYABLE(mapped_region)

 public:
  //!Creates a mapping region of the mapped memory "mapping", starting in
  //!offset "offset", and the mapping's size will be "size". The mapping
  //!can be opened for read only, read-write or copy-on-write.
  //!
  //!If an address is specified, both the offset and the address must be
  //!multiples of the page size.
  //!
  //!The map is created using "default_map_options". This flag is OS
  //!dependant and it should not be changed unless the user needs to
  //!specify special options.
  //!
  //!In UNIX systems and POSIX mappings "map_options" is an int value passed as "flags"
  //!to "mmap". If "default_map_options" is specified it's initialized to MAP_NOSYNC
  //!if that option exists and to zero otherwise. "map_options" XORed with MAP_PRIVATE or MAP_SHARED.
  //!
  //!In UNIX systems and XSI mappings "map_options" is an int value passed as "shmflg"
  //!to "shmat". If "default_map_options" is specified it's initialized to zero.
  //!"map_options" is XORed with SHM_RDONLY if needed.
  //!
  //!The OS could allocate more pages than size/page_size(), but get_address()
  //!will always return the address passed in this function (if not null) and
  //!get_size() will return the specified size.
  template<class MemoryMappable>
  mapped_region(const MemoryMappable& mapping,
                mode_t mode, offset_t offset = 0,
                std::size_t size = 0, const void* address = 0,
                map_options_t map_options = default_map_options);

};

}
}
}


#endif //AUTODDS_LIBS_INTERPROCESS_MAPPED_REGION_HPP_