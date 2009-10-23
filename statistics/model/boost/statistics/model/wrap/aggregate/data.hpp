///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::aggregate::data.hpp                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_DATA_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_DATA_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/covariate.hpp>
#include <boost/statistics/model/wrap/unary/response.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename X,typename Y>
    class data_ : 
        public covariate_wrapper<X>, 
        public response_wrapper<Y>
    {
        typedef covariate_wrapper<X> cov_;
        typedef response_wrapper<Y> resp_;

    public:
        typedef data_<X,Y> data__type;
        
        // Construction
        data_();
        data_(cov_ cov,resp_ resp);
        data_(const data_&);
            
    };

    // Implementation //
            
    template<typename X,typename Y>
    data_<X,Y>::data_() : cov_(),resp_(){}
        
    template<typename X,typename Y>
    data_<X,Y>::data_( cov_ cov, resp_ resp ) 
    : cov_( cov ), resp_( resp ){}
        
    template<typename X,typename Y>
    data_<X,Y>::data_(const data_& that)
    :cov_( that ),resp_( that ) {}

    // Free functions
        
    template<typename X,typename Y>
    data_<X,Y> 
    make_data(const X& x,const Y& y){
        typedef data_<X,Y> result_type;
        return result_type(x,y);
    }

}// model    
}// statistics
}// boost

#endif
