//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::functional::call_marginal_estimate.hpp      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_FUNCTIONAL_CALL_MARGINAL_ESTIMATE_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_FUNCTIONAL_CALL_MARGINAL_ESTIMATE_HPP_ER_2009

namespace boost{
namespace statistics{
namespace goodness_of_fit{

    struct call_marginal_estimate{
    
        call_marginal_estimate(){}
        
        template<typename E,typename X,typename X1>
        void operator()(const E& e,const X& x,X1& x1)const{
            x1 = statistics::estimate(e,x);
        }

    };

}// goodness_of_fit
}// statistics
}// boost

#endif
