//
// Created by wz on 24-2-2.
//

#ifndef AUTODDS_LIBS_MOVE_DETAIL_STD_NS_BEGIN_HPP_
#define AUTODDS_LIBS_MOVE_DETAIL_STD_NS_BEGIN_HPP_

#if defined(_LIBCPP_VERSION)
    #if defined(__clang__)
      #define AUTODDS_MOVE_STD_NS_GCC_DIAGNOSTIC_PUSH
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wc++11-extensions"
   #endif
   #define AUTODDS_MOVE_STD_NS_BEG _LIBCPP_BEGIN_NAMESPACE_STD
   #define AUTODDS_MOVE_STD_NS_END _LIBCPP_END_NAMESPACE_STD
#elif defined(AUTODDS_GNU_STDLIB) && defined(_GLIBCXX_BEGIN_NAMESPACE_VERSION)  //GCC >= 4.6
   #define AUTODDS_MOVE_STD_NS_BEG namespace std _GLIBCXX_VISIBILITY(default) { _GLIBCXX_BEGIN_NAMESPACE_VERSION
   #define AUTODDS_MOVE_STD_NS_END _GLIBCXX_END_NAMESPACE_VERSION  } // namespace
#elif defined(AUTODDS_GNU_STDLIB) && defined(_GLIBCXX_BEGIN_NAMESPACE)  //GCC >= 4.2
   #define AUTODDS_MOVE_STD_NS_BEG _GLIBCXX_BEGIN_NAMESPACE(std)
   #define AUTODDS_MOVE_STD_NS_END _GLIBCXX_END_NAMESPACE
#else
   #define AUTODDS_MOVE_STD_NS_BEG namespace std{
   #define AUTODDS_MOVE_STD_NS_END }
#endif

#endif //AUTODDS_LIBS_MOVE_DETAIL_STD_NS_BEGIN_HPP_