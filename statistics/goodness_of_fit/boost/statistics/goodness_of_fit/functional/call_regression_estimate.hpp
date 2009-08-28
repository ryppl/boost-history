//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::functional::call_regression_estimate.hpp    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_FUNCTIONAL_CALL_REGRESSION_ESTIMATE_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_FUNCTIONAL_CALL_REGRESSION_ESTIMATE_HPP_ER_2009

namespace boost{
namespace statistics{
namespace goodness_of_fit{

    struct call_regression_estimate{
    
        call_regression_estimate(){}
        
        template<typename E,typename T,typename Y>
        void operator()(const E& e,const T& xy,Y& y)const{
            y = statistics::estimate(
                e,
                boost::get<0>(xy)
            );
        }

    };

}// goodness_of_fit
}// statistics
}// boost

#endif



