//
// Created by wz on 24-2-19.
//

#ifndef AUTODDS_LIBS_TYPE_TRAITS_YES_NO_TYPE_HPP_
#define AUTODDS_LIBS_TYPE_TRAITS_YES_NO_TYPE_HPP_

namespace autodds {
namespace libs {
namespace type_traits {

typedef char yes_tpye;
struct no_type
{ char padding[8]; };

}
}
}

#endif //AUTODDS_LIBS_TYPE_TRAITS_YES_NO_TYPE_HPP_