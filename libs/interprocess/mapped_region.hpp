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

  //!Default constructor. Address will be 0 (nullptr).
  //!Size will be 0.
  //!Does not throw
  mapped_region() AUTODDS_NOEXCEPT;

  //!Move constructor. *this will be constructed taking ownership of "other"'s
  //!region and "other" will be left in default constructor state.
  mapped_region(AUTODDS_RV_REF(mapped_region) other)  AUTODDS_NOEXCEPT
      : m_base(0), m_size(0), m_page_offset(0), m_mode(read_only), m_is_xsi(false)
  { this->swap(other); }

  //!Destroys the mapped region.Does not throw
  ~mapped_region();

  //!Move assignment. If *this owns a memory mapped region, it will be
  //!destroyed and it will take ownership of "other"'s memory mapped region.
  mapped_region& operator = ( AUTODDS_RV_REF(mapped_region) other) AUTODDS_NOEXCEPT
  {
    mapped_region tmp(autodds::libs::move(other));
    this->swap(other);
    return *this;
  }

  // swap mapped region with others
  void swap(mapped_region& other) AUTODDS_NOEXCEPT;

  // size of the mapping region , never throws
  std::size_t get_size() const AUTODDS_NOEXCEPT;

  // return base address of the mapping region, never throws
  void* get_address() const AUTODDS_NOEXCEPT;

  // return the mode of the mapping region, never throws
  mode_t get_mode() const AUTODDS_NOEXCEPT;

  // Flushes to the disk a byte range within the mapped memory.
  // If 'async' is true, the function will return before flushing operation is completed
  // If 'async' is false, function will return once data has been written into the underlying
  // device (i.e., in mapped files OS cached information is written to disk).
  // Never throws. Returns false if operation could not be performed.
  bool flush(std::size_t mapping_offset = 0, std::size_t numbytes = 0, bool async = true);

  // Shrinks current mapped region. If after shrinking there is no longer need for a previously
  // mapped memory page, accessing that page can trigger a segmentation fault.
  // Depending on the OS, this operation might fail (XSI shared memory), it can decommit storage
  // and free a portion of the virtual address space (e.g.POSIX) or this
  bool shrink_by(std::size_t bytes, bool from_back = true);

  // This enum specifies region usage behaviors that an application can specify
  // to the mapped region implementation.
  enum advice_types {
    // Specifies that the application has no advice to give on its behavior with respect to
    // the region. It is the default characteristic if no advice is given for a range of memory.
    advice_normal,
    // Specifies that the application expects to access the region sequentially from
    // lower addresses to higher addresses. The implementation can lower the priority of
    // preceding pages within the region once a page have been accessed.
    advice_sequential,
    // Specifies that the application expects to access the region in a random order,
    // and prefetching is likely not advantageous.
    advice_random,
    // Specifies that the application expects to access the region in the near future.
    // The implementation can prefetch pages of the region.
    advice_willneed,
    // Specifies that the application expects that it will not access the region in the near future.
    // The implementation can unload pages within the range to save system resources.
    advice_dontneed
  };

  // Advises the implementation on the expected behavior of the application with respect to the data
  // in the region. The implementation may use this information to optimize handling of the region data.
  // This function has no effect on the semantics of access to memory in the region, although it may affect
  // the performance of access.
  bool advise(advice_types type);

  static std::size_t get_page_size() AUTODDS_NOEXCEPT;

 private:

  //!Closes a previously opened memory mapping. Never throws
  void priv_close();

  void* priv_map_address() const;

  std::size_t priv_map_size() const;

  bool priv_flush_param_check(std::size_t mapping_offset, void*& addr, std::size_t& numbytes) const;

  bool priv_shrink_param_check(std::size_t bytes, bool from_back, void*& shrink_page_start, std::size_t& shrink_page_bytes);

  static void priv_size_from_mapping_size(offset_t mapping_size, offset_t offset, offset_t page_offset, std::size_t &size);

  static offset_t priv_page_offset_addr_fixup(offset_t page_offset, const void *&addr);

  template<int d>
  struct page_size_holder
  {
    static const std::size_t pg_size;
    static std::size_t get_page_size();
  };

  void*               m_base;
  std::size_t         m_size;
  std::size_t         m_page_offset;
  mode_t              m_mode;
  bool                m_is_xsi;

  friend class ipcdetail::interprocess_tester;
  friend class ipcdetail::raw_mapped_region_creator;

  void dont_close_on_destruction();

};

inline void swap(mapped_region& x, mapped_region& y) AUTODDS_NOEXCEPT
{ x.swap(y); }

inline mapped_region::~mapped_region() AUTODDS_NOEXCEPT
{ this->priv_close(); }

inline std::size_t mapped_region::get_size() const AUTODDS_NOEXCEPT
{ return m_size; }

inline mode_t mapped_region::get_mode() const AUTODDS_NOEXCEPT
{ return m_mode; }

inline void* mapped_region::get_address() const AUTODDS_NOEXCEPT
{ return m_base; }

inline void* mapped_region::priv_map_address() const
{ return static_cast<char*>(m_base) - m_page_offset; }

inline std::size_t mapped_region::priv_map_size() const
{ return m_size + m_page_offset; }

inline bool mapped_region::priv_flush_param_check(std::size_t mapping_offset,
                                                  void *&addr,
                                                  std::size_t &numbytes) const
{
  if (m_base == 0) return false;
  if (mapping_offset >= m_size || numbytes > (m_size - size_t(mapping_offset)))
  {
    return false;
  }
  if (numbytes == 0) {
    numbytes = m_size - mapping_offset;
  }

  addr = static_cast<char*>(this->priv_map_address()) + mapping_offset;
  numbytes += m_page_offset;
  return true;
}

inline bool mapped_region::priv_shrink_param_check
    (std::size_t bytes, bool from_back, void *& shrink_page_start, std::size_t &shrink_page_bytes)
{
  if(m_base == 0 || bytes > m_size){
    return false;
  } else if (bytes == m_size) {
    this->priv_close();
  } else {
    const std::size_t page_size = mapped_region::get_page_size();
    if (from_back) {
      const std::size_t new_pages = (m_size + m_page_offset - bytes -1)/page_size + 1;
      shrink_page_start = static_cast<char*>(this->priv_map_address()) + new_pages*page_size;
      shrink_page_bytes = m_page_offset + m_size - new_pages * page_size;
      m_size -= bytes;
    } else
    {
      shrink_page_start = this->priv_map_address();
      m_page_offset += bytes;
      shrink_page_bytes = (m_page_offset / page_size) * page_size;
      m_page_offset = m_page_offset % page_size;
      m_size -= bytes;
      m_base = static_cast<char*>(m_base) + bytes;
      AUTODDS_ASSERT(shrink_page_bytes % page_size == 0);
    }
    return true;
  }
}

inline void mapped_region::priv_size_from_mapping_size
    (offset_t mapping_size,  offset_t offset, offset_t page_offset, std::size_t &size)
{
  //Check if mapping size fits in the user address space
  //as offset_t is the maximum file size and it's signed.
  if (mapping_size < offset ||
      autodds::libs::uintmax_t(mapping_size - (offset - page_offset)) >
          autodds::libs::uintmax_t(std::size_t(-1))) {
    error_info err(size_error);
    throw interprocess_exception(err);
  }
  size = static_cast<std::size_t>(mapping_size - offset);
}

inline offset_t mapped_region::priv_page_offset_addr_fixup(autodds::libs::interprocess::offset_t offset,
                                                           const void *&addr) {
  const std::size_t page_size  = mapped_region::get_page_size();

  //We calculate the difference between demanded and valid offset
  const std::size_t page_offset = static_cast<std::size_t>(
      offset - (offset / offset_t(page_size)) * offset_t(page_size));
  if (addr) {
    addr = static_cast<const char*>(addr) - page_offset;
  }
  return offset_t(page_offset);
}

inline mapped_region::mapped_region() AUTODDS_NOEXCEPT
    :  m_base(0), m_size(0), m_page_offset(0), m_mode(read_only), m_is_xsi(false)
{}

template<int d>
inline std::size_t mapped_region::page_size_holder<d>::get_page_size()
{ return std::size_t(sysconf(_SC_PAGESIZE)); }

template<class MemoryMappable>
inline mapped_region::mapped_region(const MemoryMappable &mapping,
                                    autodds::libs::interprocess::mode_t mode,
                                    autodds::libs::interprocess::offset_t offset,
                                    std::size_t size,
                                    const void *address,
                                    autodds::libs::interprocess::map_options_t map_options)
    : m_base(0), m_size(0), m_page_offset(0),m_mode(mode),m_is_xsi(false)
{
  mapping_handle_t map_handle = mapping.get_mapping_handle();
#ifdef AUTODDS_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS
  if (map_handle.is_xsi) {
    ::shmid_ds xsi_ds;
    int ret = ::shmctl(map_handle.handle, IPC_STAT, &xsi_ds);
    if (ret == -1) {
      error_info err(system_error_code());
      throw interprocess_exception(err);
    }

    // Compare sizes
    if (size == 0) {
      size = (std::size_t)xsi_ds.shm_segsz;
    }
    else if (size != (std::size_t)xsi_ds.shm_segsz) {
      error_info err(size_error);
      throw interprocess_exception(err);
    }

    // Calculate flag
    int flag = map_options == default_map_options ? 0 : map_options;
    if (m_mode == read_only) {
      flag |= SHM_RDONLY;
    }
    else if (m_mode != read_write) {
      error_info err(mode_error);
      throw interprocess_exception(err);
    }

    //Attach memory
    void *const final_address = const_cast<void*>(address);
    void *base = ::shmat(map_handle.handle,final_address,flag);

    if(base == (void*)-1){
      error_info err(system_error_code());
      throw interprocess_exception(err);
    }
    m_base   = base;
    m_size   = size;
    m_mode   = mode;
    m_page_offset = 0;
    m_is_xsi = true;
    return;
  }
#endif
  //We calculate the difference between demanded and valid offset
  const offset_t page_offset = priv_page_offset_addr_fixup(offset, address);

  if(size == 0){
    struct ::stat buf;
    if(0 != fstat(map_handle.handle, &buf)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
    }
    //This can throw
    priv_size_from_mapping_size(buf.st_size, offset, page_offset, size);
  }

#ifdef MAP_NOSYNC
#define AUTODDS_INTERPROCESS_MAP_NOSYNC MAP_NOSYNC
#else
#define AUTODDS_INTERPROCESS_MAP_NOSYNC 0
#endif   //MAP_NOSYNC

  // Create new mapping
  int prot = 0;
  int flags = map_options == default_map_options ? AUTODDS_INTERPROCESS_MAP_NOSYNC : map_options;

#undef AUTODDS_INTERPROCESS_MAP_NOSYNC

  switch (mode) {

    case read_only:
      prot |= PROT_READ;
      flags |= MAP_SHARED;
      break;

    case read_private:
      prot |= PROT_READ;
      flags |= MAP_PRIVATE;
      break;

    case read_write:
      prot |= (PROT_WRITE | PROT_READ);
      flags |= MAP_SHARED;
      break;

    case copy_on_write:
      prot |= (PROT_WRITE | PROT_READ);
      flags |= MAP_PRIVATE;
      break;

    default:
    {
      error_info err(mode_error);
      throw interprocess_exception(err);
    }
      break;
  }

  //Map it to the address space
  void* base = mmap(const_cast<void*>(address),
                    static_cast<std::size_t>(page_offset) + size,
                    prot, flags,
                    mapping.get_mapping_handle().handle,
                    offset - page_offset);

  if (base == MAP_FAILED) {
    error_info err = system_error_code();
    throw interprocess_exception(err);
  }
  //Calculate new base for the user
  m_base = static_cast<char*>(base) + page_offset;
  m_page_offset = static_cast<std::size_t>(page_offset);
  m_size = size;

  //Check for fixed mapping error
  if(address && (base != address)){
    error_info err(busy_error);
    this->priv_close();
    throw interprocess_exception(err);
  }
}


}
}
}


#endif //AUTODDS_LIBS_INTERPROCESS_MAPPED_REGION_HPP_