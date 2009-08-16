///////////////////////////////////////////////////////////////////////////////
// joint_dist::meta::is_joint_dist.hpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_FUN_WRAP_META_IS_JOINT_DIST_ER_2009
#define BOOST_JOINT_DIST_FUN_WRAP_META_IS_JOINT_DIST_ER_2009
#include <boost/mpl/bool.hpp>

namespace boost{
namespace joint_dist{

    template<typename D>
    struct is_joint_dist : mpl::bool_<false> {};

    // What for? Manage overload set externally

}// joint_dist
}// boost

#endif


