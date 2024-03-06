//
// Created by wz on 24-3-6.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_EBO_FUNCTOR_HOLDER_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_EBO_FUNCTOR_HOLDER_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/intrusive/detail/workaround.hpp"
#include "libs/move/utility_core.hpp"

namespace autodds {
namespace libs {
namespace intrusive {
namespace detail {

#define AUTODDS_INTRUSIVE_TT_DECL

template<typename T>
struct is_unary_or_binary_function_impl
{ static const bool value = false; };

template <typename R>
struct is_unary_or_binary_function_impl<R (*)()>
{  static const bool value = true;  };

template <typename R>
struct is_unary_or_binary_function_impl<R (*)(...)>
{  static const bool value = true;  };

template <typename R, typename T0>
struct is_unary_or_binary_function_impl<R (*)(T0)>
{  static const bool value = true;  };

template <typename R, typename T0>
struct is_unary_or_binary_function_impl<R (*)(T0...)>
{  static const bool value = true;  };

template <typename R, typename T0, typename T1>
struct is_unary_or_binary_function_impl<R (*)(T0, T1)>
{  static const bool value = true;  };

template <typename R, typename T0, typename T1>
struct is_unary_or_binary_function_impl<R (*)(T0, T1...)>
{  static const bool value = true;  };

template <typename T>
struct is_unary_or_binary_function_impl<T&>
{  static const bool value = false; };

template<typename T>
struct is_unary_or_binary_function : is_unary_or_binary_function_impl<T>
{};

template<typename T, typename Tag = void, bool = is_unary_or_binary_function<T>::value>
class ebo_functor_holder
{
  AUTODDS_COPYABLE_AND_MOVABLE_ALT(ebo_functor_holder)

 public:
  typedef T functor_type;

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder()
      : t_()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit ebo_functor_holder(const T& t)
      : t_(t)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit ebo_functor_holder(AUTODDS_RV_REF(T) t)
      : t_(autodds::libs::move(t))
  {}

  template<typename U1, typename U2>
  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(AUTODDS_FWD_REF(U1) arg1, AUTODDS_FWD_REF(U2) arg2)
      : t_(::autodds::libs::forward<U1>(arg1), ::autodds::libs::forward<U2>(arg2))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(AUTODDS_LV_CONST_REF(ebo_functor_holder, x))
      : t_(x.t_)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(AUTODDS_RV_REF(ebo_functor_holder) x)
      : t_(x.t_)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(AUTODDS_COPY_ASSIGN_REF(ebo_functor_holder) x)
  {
    this->get() = x.get();
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(AUTODDS_RV_REF(ebo_functor_holder) x)
  {
    this->get() = ::autodds::libs::move(x.get());
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator=(const T &x)
  {
    this->get() = x;
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator=(AUTODDS_RV_REF(T) x)
  {
    this->get() = ::autodds::libs::move(x);
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE T&       get(){return t_;}
  AUTODDS_INTRUSIVE_FORCEINLINE const T& get()const{return t_;}

 private:
  T t_;
};

template<typename T, typename Tag>
class ebo_functor_holder<T, Tag, false> : public T
{
  AUTODDS_COPYABLE_AND_MOVABLE(ebo_functor_holder)

 public:
  typedef T functor_type;

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder()
      : T()
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit ebo_functor_holder(const T &t)
      : T(t)
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE explicit ebo_functor_holder(AUTODDS_RV_REF(T) t)
      : T(::autodds::libs::move(t))
  {}

  template<class U1, class U2>
  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(AUTODDS_FWD_REF(U1) arg1, AUTODDS_FWD_REF(U1) arg2)
      : T(::autodds::libs::forward<U1>(arg1), ::autodds::libs::forward<U2>(arg2))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(const ebo_functor_holder &x)
      : T(static_cast<const T&>(x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder(AUTODDS_RV_REF(ebo_functor_holder) x)
      : T(AUTODDS_MOVE_BASE(T, x))
  {}

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(AUTODDS_COPY_ASSIGN_REF(ebo_functor_holder) x)
  {
    const ebo_functor_holder&r = x;
    this->get() = r;
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(AUTODDS_RV_REF(ebo_functor_holder) x)
  {
    this->get() = ::autodds::libs::move(x.get());
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(const T &x)
  {
    this->get() = x;
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE ebo_functor_holder& operator =(AUTODDS_RV_REF(T) x)
  {
    this->get() = ::autodds::libs::move(x);
    return *this;
  }

  AUTODDS_INTRUSIVE_FORCEINLINE T&       get(){return *this;}
  AUTODDS_INTRUSIVE_FORCEINLINE const T& get()const{return *this;}

};


} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_EBO_FUNCTOR_HOLDER_HPP_