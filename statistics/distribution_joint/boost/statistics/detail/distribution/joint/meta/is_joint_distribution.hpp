///////////////////////////////////////////////////////////////////////////////
// distribution::joint::meta::is_joint_distribution.hpp                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_META_IS_JOINT_DISTRIBUTION_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_META_IS_JOINT_DISTRIBUTION_ER_2009
#include <boost/mpl/bool.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename D>
    struct is_joint_distribution : mpl::bool_<false> {};


}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif


