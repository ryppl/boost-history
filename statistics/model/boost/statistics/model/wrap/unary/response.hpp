///////////////////////////////////////////////////////////////////////////////
// model::wrap::unary::response.hpp                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_RESPONSE_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_RESPONSE_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename Y>
    class response_ : detail::base_<Y>{
        typedef detail::base_<Y> base_;
    public:
        typedef Y                 response_type;
        typedef response_<Y>      response_w_;
            
        // Construction
        response_();
        response_(const Y& y);
        response_(const response_&);
            
        // Access
        const Y& response()const;
    };

    // Implementation //
            
    template<typename Y>
    response_<Y>::response_() : base_(){}
        
    template<typename Y>
    response_<Y>::response_( const Y& y ) : base_(y){}
        
    template<typename Y>
    response_<Y>::response_(const response_& that)
    :base_(that){}

    template<typename Y>
    const Y& response_<Y>::response()const{ return (this->value); }
                
    // Free functions
        
    template<typename Y>
    response_<Y> 
    make_response(const Y& y){
        typedef response_<Y> result_type;
        return result_type(y);
    }

}// model    
}// statistics
}// boost

#endif
