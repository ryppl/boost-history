///////////////////////////////////////////////////////////////////////////////
// distribution::joint::meta::pointer.hpp                                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_META_POINTER_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_META_POINTER_ER_2009
#include <boost/statistics/detail/distribution/joint/meta/signature.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename D,typename R>
    struct pointer{
        typedef signature<D,R> meta_sig_;
        typedef typename meta_sig_::type sig_;
        typedef sig_* type;
    };

}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif