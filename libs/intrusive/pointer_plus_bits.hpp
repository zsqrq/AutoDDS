//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_POINTER_PLUS_BITS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_POINTER_PLUS_BITS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/detail/mpl.hpp"
#include "libs/intrusive/detail/assert.hpp"

#if defined(AUTODDS_GCC)
#  if (AUTODDS_GCC >= 40600)
#     pragma GCC diagnostic push
#     pragma GCC diagnostic ignored "-Wuninitialized"
#     if (AUTODDS_GCC >= 40700)
#        pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#     endif
#  endif
#endif

namespace autodds {
namespace libs {
namespace intrusive {

//!This trait class is used to know if a pointer
//!can embed extra bits of information if
//!it's going to be used to point to objects
//!with an alignment of "Alignment" bytes.
template<typename VoidPointer, std::size_t Alignment>
struct max_pointer_plus_bits
{
  static const std::size_t value = 0;
};

//!This is a specialization for raw pointers.
//!Raw pointers can embed extra bits in the lower bits
//!if the alignment is multiple of 2pow(NumBits).
template<std::size_t Alignment>
struct max_pointer_plus_bits<void*, Alignment>
{
  static const std::size_t value = ::autodds::intrusive::detail::ls_zeros<Alignment>::value;
};

//!This is class that is supposed to have static methods
//!to embed extra bits of information in a pointer.
//!This is a declaration and there is no default implementation,
//!because operations to embed the bits change with every pointer type.
//!
//!An implementation that detects that a pointer type whose
//!has_pointer_plus_bits<>::value is non-zero can make use of these
//!operations to embed the bits in the pointer.
template<typename Pointer, std::size_t NumBits>
struct pointer_plus_bits;

//!This is the specialization to embed extra bits of information
//!in a raw pointer. The extra bits are stored in the lower bits of the pointer.
template<typename T, std::size_t NumBits>
struct pointer_plus_bits<T*, NumBits>
{
  static const uintptr_t Mask = uintptr_t((uintptr_t(1u) << NumBits) - 1);
  typedef T*   pointer;

  AUTODDS_INTRUSIVE_FORCEINLINE static pointer get_pointer(pointer n) AUTODDS_NOEXCEPT
  {
    pointer(uintptr_t(n) & uintptr_t(~Mask));
  }

};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_POINTER_PLUS_BITS_HPP_