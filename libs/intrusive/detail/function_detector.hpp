//
// Created by wz on 24-3-7.
//

#ifndef AUTODDS_LIBS_INTRUSIVE_DETAIL_FUNCTION_DETECTOR_HPP_
#define AUTODDS_LIBS_INTRUSIVE_DETAIL_FUNCTION_DETECTOR_HPP_

#ifndef  AUTODDS_CONFIG_HPP
#  include "libs/config/config.hpp"
#endif
#

#if defined (AUTODDS_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace autodds {
namespace libs {
namespace intrusive {
namespace function_detector {

typedef char NotFoundType;
struct StaticFunctionType { NotFoundType x [2]; };
struct NonStaticFunctionType { NotFoundType x [3]; };

enum
{ NotFound          = 0,
  StaticFunction    = sizeof( StaticFunctionType )    - sizeof( NotFoundType ),
  NonStaticFunction = sizeof( NonStaticFunctionType ) - sizeof( NotFoundType )
};

} // namespace detail
} // namespace intrusive
} // namespace libs
} // namespace autodds

#define AUTODDS_INTRUSIVE_CREATE_FUNCTION_DETECTOR(Identifier, InstantiationKey) \
   namespace autodds {                                                           \
   namespace libs {                                                            \
   namespace intrusive { \
   namespace function_detector { \
   template < class T, \
            class NonStaticType, \
            class NonStaticConstType, \
            class StaticType > \
   class DetectMember_##InstantiationKey_##Identifier { \
      template < NonStaticType > \
      struct TestNonStaticNonConst ; \
      \
      template < NonStaticConstType > \
      struct TestNonStaticConst ; \
      \
      template < StaticType > \
      struct TestStatic ; \
      \
      template <class U > \
      static NonStaticFunctionType Test( TestNonStaticNonConst<&U::Identifier>*, int ); \
      \
      template <class U > \
      static NonStaticFunctionType Test( TestNonStaticConst<&U::Identifier>*, int ); \
      \
      template <class U> \
      static StaticFunctionType Test( TestStatic<&U::Identifier>*, int ); \
      \
      template <class U> \
      static NotFoundType Test( ... ); \
   public : \
      static const int check = NotFound + (sizeof(Test<T>(0, 0)) - sizeof(NotFoundType));\
   };\
}}}}

#define AUTODDS_INTRUSIVE_DETECT_FUNCTION(Class, InstantiationKey, ReturnType, Identifier, Params) \
    ::autodds::libs::intrusive::function_detector::DetectMember_##InstantiationKey_##Identifier< Class,\
                                         ReturnType (Class::*)Params,\
                                         ReturnType (Class::*)Params const,\
                                         ReturnType (*)Params \
                                       >::check

#endif //AUTODDS_LIBS_INTRUSIVE_DETAIL_FUNCTION_DETECTOR_HPP_