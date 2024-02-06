//
// Created by wz on 24-2-6.
//

#ifndef AUTODDS_LIBS_MOVE_CORE_HPP_
#define AUTODDS_LIBS_MOVE_CORE_HPP_

#ifndef AUTODDS_CONFIG_HPP
#  include "libs/config//config.hpp"
#endif
#if defined(AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "libs/move/detail/workaround.hpp"

#define AUTODDS_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE) \
      public:\
      TYPE(TYPE const &) = delete;\
      TYPE& operator=(TYPE const &) = delete;\
      public:\
      typedef int autodds_move_no_copy_constructor_or_assign; \
      private:\
   //

#define AUTODDS_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      AUTODDS_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE)\
      public:\
      typedef int autodds_move_emulation_t;\
      private:\
   //

#define AUTODDS_COPYABLE_AND_MOVABLE(TYPE)\
   //

#define AUTODDS_COPYABLE_AND_MOVABLE_ALT(TYPE)\
   //

namespace autodds {
namespace libs {

template <typename T>
struct has_move_emulation_enabled {
  static const bool value = false;
};

template <typename T>
struct has_move_emulation_disabled {
  static const bool value = true;
};

}
}

#define AUTODDS_RV_REF(TYPE)\
      TYPE && \
   //
#define AUTODDS_RV_REF_BEG\
         \
   //
#define AUTODDS_RV_REF_END\
      && \
   //

#define AUTODDS_RV_REF_BEG_IF_CXX11 \
      AUTODDS_RV_REF_BEG \
   //

#define AUTODDS_RV_REF_END_IF_CXX11 \
      AUTODDS_RV_REF_END \
   //

#define AUTODDS_COPY_ASSIGN_REF(TYPE)\
      const TYPE & \
   //

#define AUTODDS_FWD_REF(TYPE)\
      TYPE && \
   //

#define AUTODDS_RV_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      TYPE<ARG1, ARG2> && \
   //

#define AUTODDS_RV_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      TYPE<ARG1, ARG2, ARG3> && \
   //

#define AUTODDS_COPY_ASSIGN_REF_BEG \
      const \
   //

#define AUTODDS_COPY_ASSIGN_REF_END \
      & \
   //

#define AUTODDS_COPY_ASSIGN_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      const TYPE<ARG1, ARG2> & \
   //

#define AUTODDS_COPY_ASSIGN_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      const TYPE<ARG1, ARG2, ARG3>& \
   //

#define AUTODDS_CATCH_CONST_RLVALUE(TYPE)\
      const TYPE & \
   //

#define AUTODDS_MOVE_RET(RET_TYPE, REF)\
         REF
//

#define AUTODDS_MOVE_BASE(BASE_TYPE, ARG) \
      ::autodds::libs::move((BASE_TYPE&)(ARG))
//

#define AUTODDS_MOVE_TO_LV(ARG) ARG
//

namespace autodds {
namespace libs {
namespace move_detail {

template <typename T>
struct forward_type { typedef T type; };

}
}
}

#endif //AUTODDS_LIBS_MOVE_CORE_HPP_