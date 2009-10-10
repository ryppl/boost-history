///////////////////////////////////////////////////////////////////////////////
// distribution::joint::fun_wrap::cdf::detail::fun_wrap.hpp                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_DETAIL_FUN_WRAP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_DETAIL_FUN_WRAP_ER_2009
#include <boost/statistics/detail/distribution/joint/meta/pointer.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{
namespace fun_wrap{
namespace detail{

    template<typename D,typename R,typename pointer<D,R>::type f>
    struct fun_wrap{
        typedef function<D> fun_;
        typedef typename pointer<D,R>::type type;
        static type instance;
    };

    template<typename D,typename R,typename pointer<D,R>::type f>
    typename fun_wrap<D,R,f>::type
    fun_wrap<D,R,f>::instance = f;

}// detail
}// fun_wrap
}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
