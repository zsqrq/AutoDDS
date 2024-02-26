//
// Created by wz on 24-2-23.
//

#ifndef AUTODDS_LIBS_INTERPROCESS_DETAIL_ATOMIC_HPP_
#define AUTODDS_LIBS_INTERPROCESS_DETAIL_ATOMIC_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/interprocess/detail/config_begin.hpp"
#include "libs/interprocess/detail/workaround.hpp"
#include "libs/common/cstdint.hpp"

#if !defined(_AIX)
#define AUTODDS_INTERPROCESS_DETAIL_PPC_ASM_LABEL(label) label ":\n\t"
#define AUTODDS_INTERPROCESS_DETAIL_PPC_ASM_JUMP(insn, label, offset) insn " " label "\n\t"
#else
#define AUTODDS_INTERPROCESS_DETAIL_PPC_ASM_LABEL(label)
#define AUTODDS_INTERPROCESS_DETAIL_PPC_ASM_JUMP(insn, label, offset) insn " $" offset "\n\t"
#endif

#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) && !defined(_CRAYC)


namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

//! Atomically increment an autodds::libs::uint32_t by 1
//! "mem": pointer to the object
//! Returns the old value pointed to by mem
inline autodds::libs::uint32_t  atomic_inc32(volatile autodds::libs::uint32_t* mem);

inline autodds::libs::uint32_t atomic_read32(volatile autodds::libs::uint32_t* mem);

//! Atomically set an autodds::libs::uint32_t in memory
//! "mem": pointer to the object
//! "param": val value that the object will assume
inline void atomic_write32(volatile autodds::libs::uint32_t* mem, autodds::libs::uint32_t val);

//! Compare an boost::uint32_t's value with "cmp".
//! If they are the same swap the value with "with"
//! "mem": pointer to the value
//! "with": what to swap it with
//! "cmp": the value to compare it to
//! Returns the old value of *mem
inline autodds::libs::uint32_t atomic_cas32(volatile autodds::libs::uint32_t* mem,
                                            autodds::libs::uint32_t with, autodds::libs::uint32_t cmp)
{
  autodds::libs::uint32_t prev = cmp;

  asm volatile
      (
      "lock\n\t"
      "cmpxchg %2,%0"
      : "+m"(*mem), "+a"(prev)
      : "r"(with)
      : "cc"
      );
  return prev;
}

//! Atomically add 'val' to an boost::uint32_t
//! "mem": pointer to the object
//! "val": amount to add
//! Returns the old value pointed to by mem
inline autodds::libs::uint32_t atomic_add32(volatile autodds::libs::uint32_t *mem, autodds::libs::uint32_t val)
{
  autodds::libs::uint32_t r;
  asm volatile
      (
      "lock\n\t"
      "xadd %1, %0":
      "+m"( *mem ), "=r"( r ): // outputs (%0, %1)
      "1"( val ): // inputs (%2 == %1)
      "memory", "cc" // clobbers
      );
  return r;
}

inline autodds::libs::uint32_t  atomic_inc32(volatile autodds::libs::uint32_t* mem)
{ return atomic_add32(mem, 1); }

inline autodds::libs::uint32_t atomic_dec32(volatile autodds::libs::uint32_t* mem)
{ return atomic_add32(mem, static_cast<autodds::libs::uint32_t>(-1)); }

inline autodds::libs::uint32_t atomic_read32(volatile autodds::libs::uint32_t* mem)
{
  const autodds::libs::uint32_t val =  *mem;
  asm volatile ("" ::: "memory");
  return val;
}

inline void atomic_write32(volatile autodds::libs::uint32_t* mem, autodds::libs::uint32_t val)
{
  asm volatile
      (
          "xchgl %0, %1"
          : "+r" (val), "+m" (*mem)
          :: "memory"
      );
}


} // namespace ipcdetail
} // namespace libs
} // namespace interprocess
} // namespace autodds

#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

inline autodds::libs::uint32_t atomic_add32
   (volatile autodds::libs::uint32_t *mem, autodds::libs::uint32_t val)
{  return __sync_fetch_and_add(const_cast<autodds::libs::uint32_t *>(mem), val);   }

//! Atomically increment an apr_uint32_t by 1
//! "mem": pointer to the object
//! Returns the old value pointed to by mem
inline autodds::libs::uint32_t atomic_inc32(volatile autodds::libs::uint32_t *mem)
{  return atomic_add32(mem, 1);  }

//! Atomically decrement an boost::uint32_t by 1
//! "mem": pointer to the atomic value
//! Returns the old value pointed to by mem
inline autodds::libs::uint32_t atomic_dec32(volatile autodds::libs::uint32_t *mem)
{  return atomic_add32(mem, (autodds::libs::uint32_t)-1);   }

//! Atomically read an boost::uint32_t from memory
inline autodds::libs::uint32_t atomic_read32(volatile autodds::libs::uint32_t *mem)
{  boost::uint32_t old_val = *mem; __sync_synchronize(); return old_val;  }

//! Compare an boost::uint32_t's value with "cmp".
//! If they are the same swap the value with "with"
//! "mem": pointer to the value
//! "with" what to swap it with
//! "cmp": the value to compare it to
//! Returns the old value of *mem
inline autodds::libs::uint32_t atomic_cas32
   (volatile autodds::libs::uint32_t *mem, autodds::libs::uint32_t with, autodds::libs::uint32_t cmp)
{  return __sync_val_compare_and_swap(const_cast<autodds::libs::uint32_t *>(mem), cmp, with);   }

//! Atomically set an boost::uint32_t in memory
//! "mem": pointer to the object
//! "param": val value that the object will assume
inline void atomic_write32(volatile autodds::libs::uint32_t *mem, autodds::libs::uint32_t val)
{  __sync_synchronize(); *mem = val;  }

} // namespace ipcdetail
} // namespace libs
} // namespace interprocess
} // namespace autodds

#else

#error No atomic operations implemented for this platform, sorry!

#endif

namespace autodds {
namespace libs {
namespace interprocess {
namespace ipcdetail {

inline bool atomic_add_unless32(volatile autodds::libs::uint32_t* mem, autodds::libs::uint32_t value,
                                autodds::libs::uint32_t unless_this)
{
  autodds::libs::uint32_t old, c(atomic_read32(mem));
  while (c != unless_this && (old = atomic_cas32(mem, c + valuem c)) != c)
  {
    c = old;
  }
  return c != unless_this;
}

} // namespace ipcdetail
} // namespace libs
} // namespace interprocess
} // namespace autodds

#endif //AUTODDS_LIBS_INTERPROCESS_DETAIL_ATOMIC_HPP_