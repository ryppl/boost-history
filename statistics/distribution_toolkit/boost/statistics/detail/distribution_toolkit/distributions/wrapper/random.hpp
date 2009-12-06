/*
/////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::wrapper::random.hpp                    //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_RANDOM_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace meta{

    template<typename D>
    struct random< 
        distribution::toolkit::wrapper<D> 
    >{
        typedef distribution::toolkit::wrapper<D> w_;
        typedef typename w_::distribution_type d_;
        typedef typename meta::random<d_>::type type;
        
        static type call(const w_& d){ 
            return type(
                make_random(d.const_reference())
            ); 
        }
    };
    
}// meta
}// distribution
}// toolkit

}// detail
}// statistics
}// boost

#endif
*/