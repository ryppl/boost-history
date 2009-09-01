
#ifndef BOOST_MPL_LIST_AUX_LIST0_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_LIST0_HPP_INCLUDED

//!  
//!@nv-mpl_diff:
//!  WHAT:
//!    This replaces the list0 declaration in
//!    boost/mpl/list/list0.hpp.
//|    However, it does not replace the #include's
//!    in that file.  The #include's from that
//!    file are #included in .../list0.hpp.
//!  WHY:
//!    Make consistent with the vector files.
//!
#include <boost/mpl/list/aux_/item.hpp>

namespace boost
{
namespace mpl
{

struct list0
{
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    typedef int begin;
#endif
    typedef aux::list_tag tag;
    typedef list0 type;
    typedef list0 pkg_type;
    
    typedef long_<0> size;
};

}//exit mpl namespace
}//exit boost namespace


#endif // BOOST_MPL_LIST_AUX_LIST0_HPP_INCLUDED
