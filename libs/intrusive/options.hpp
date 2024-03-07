//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_OPTIONS_HPP_
#define AUTODDS_LIBS_INTRUSIVE_OPTIONS_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/intrusive_fwd.hpp"
#include "libs/intrusive/link_mode.hpp"
#include "libs/intrusive/pack_options.hpp"

namespace autodds {
namespace libs {
namespace intrusive {

struct empty
{};

template<class Functor>
struct fhtraits;

template<class T, class Hook, Hook T::* P>
struct mhtraits;

struct dft_tag;
struct member_tag;

template<class SupposedValueTraits>
struct is_default_hook_tag;

AUTODDS_INTRUSIVE_OPTION_CONSTANT(constant_time_size, bool, Enabled, constant_time_size)
AUTODDS_INTRUSIVE_OPTION_TYPE(header_holder_type, HeaderHolder, HeaderHolder, header_holder_type)
AUTODDS_INTRUSIVE_OPTION_TYPE(size_type, SizeType, SizeType, size_type)
AUTODDS_INTRUSIVE_OPTION_TYPE(compare, Compare, Compare, compare)
AUTODDS_INTRUSIVE_OPTION_TYPE(key_of_value, KeyOfValue, KeyOfValue, key_of_value)
AUTODDS_INTRUSIVE_OPTION_TYPE(priority_of_value, PrioOfValue, PrioOfValue, priority_of_value)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(floating_point, bool, Enabled, floating_point)
AUTODDS_INTRUSIVE_OPTION_TYPE(equal, Equal, Equal, equal)
AUTODDS_INTRUSIVE_OPTION_TYPE(priority, Priority, Priority, priority)
AUTODDS_INTRUSIVE_OPTION_TYPE(hash, Hash, Hash, hash)
AUTODDS_INTRUSIVE_OPTION_TYPE(value_traits, ValueTraits, ValueTraits, proto_value_traits)

//!This option setter specifies the member hook the
//!container must use.
template< typename Parent, typename MemberHook, MemberHook Parent::* PtrToMember>
struct member_hook
{
  typedef mhtraits <Parent, MemberHook, PtrToMember> member_value_traits;
  template<typename Base>
  struct pack : Base
  {
    typedef member_value_traits proto_value_traits;
  };
};

AUTODDS_INTRUSIVE_OPTION_TYPE(function_hook, Functor, fhtraits<Functor>, proto_value_traits)
AUTODDS_INTRUSIVE_OPTION_TYPE(base_hook, BaseHook, BaseHook, proto_value_traits)
AUTODDS_INTRUSIVE_OPTION_TYPE(void_pointer, VoidPointer, VoidPointer, void_pointer)
AUTODDS_INTRUSIVE_OPTION_TYPE(tag, Tag, Tag, tag)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(link_mode, link_mode_type, LinkType, link_mode)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(optimize_size, bool, Enabled, optimize_size)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(linear, bool, Enabled, linear)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(cache_last, bool, Enabled, cache_last)
AUTODDS_INTRUSIVE_OPTION_TYPE(bucket_traits, BucketTraits, BucketTraits, bucket_traits)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(store_hash, bool, Enabled, store_hash)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(optimize_multikey, bool, Enabled, optimize_multikey)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(power_2_buckets, bool, Enabled, power_2_buckets)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(fastmod_buckets, bool, Enabled, fastmod_buckets)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(cache_begin, bool, Enabled, cache_begin)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(compare_hash, bool, Enabled, compare_hash)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(incremental, bool, Enabled, incremental)
AUTODDS_INTRUSIVE_OPTION_CONSTANT(linear_buckets, bool, Enabled, linear_buckets)

struct hook_defaults
{
  typedef void* void_pointer;
  static const link_mode_type link_mode = safe_link;
  typedef dft_tag tag;
  static const bool optimize_size = false;
  static const bool store_hash = false;
  static const bool linear = false;
  static const bool optimize_multikey = false;
};

} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_OPTIONS_HPP_