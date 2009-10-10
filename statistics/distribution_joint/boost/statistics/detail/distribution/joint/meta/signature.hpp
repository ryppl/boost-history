///////////////////////////////////////////////////////////////////////////////
// distribution::joint::meta::signature.hpp                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_META_SIGNATURE_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_META_SIGNATURE_ER_2009
#include <boost/statistics/detail/distribution_common/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename D,typename R>
    struct signature{
        typedef typename distribution::common::meta::value<D>::type value_type;
        typedef value_type type(
            const D&,
            const R&
        );
    };
    
}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif