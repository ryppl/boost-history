///////////////////////////////////////////////////////////////////////////////
//
// is_std_list.hpp
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
# pragma once
#endif // _MSC_VER > 1000

#ifndef IS_STD_LIST_EN_16_12_2004
#define IS_STD_LIST_EN_16_12_2004

#include <boost/mpl/bool.hpp>
#include "./std_list_fwd.hpp"

namespace boost
{
    template<class T>
    struct is_std_list
        : ::boost::mpl::false_
    {};

    template<
        class Ty
      , class Alloc
    >
    struct is_std_list< ::std::list<Ty,Alloc> >
        : ::boost::mpl::true_
    {};
}

#endif
