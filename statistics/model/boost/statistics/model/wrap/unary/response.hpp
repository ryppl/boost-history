///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::response.hpp                              //
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
    class response_wrapper : detail::base_<Y>{
        typedef detail::base_<Y> base_;
    public:
        typedef Y                       response_type;
        typedef response_wrapper<Y>     response_wrapper_type;
            
        // Construction
        response_wrapper();
        response_wrapper(const Y& y);
        response_wrapper(const response_wrapper&);
            
        // Access
        const Y& response()const;
    };

    // Implementation //
            
    template<typename Y>
    response_wrapper<Y>::response_wrapper() : base_(){}
        
    template<typename Y>
    response_wrapper<Y>::response_wrapper( const Y& y ) : base_(y){}
        
    template<typename Y>
    response_wrapper<Y>::response_wrapper(const response_wrapper& that)
    :base_(that){}

    template<typename Y>
    const Y& response_wrapper<Y>::response()const{ 
        return (this->value); 
    }
                
    // Free functions
        
    template<typename Y>
    response_wrapper<Y> 
    make_response_wrapper(const Y& y){
        typedef response_wrapper<Y> result_type;
        return result_type(y);
    }

}// model    
}// statistics
}// boost

#endif
