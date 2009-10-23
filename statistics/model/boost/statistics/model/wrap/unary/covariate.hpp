///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::covariate.hpp                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_COVARIATE_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_COVARIATE_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename X>
    class covariate_wrapper : detail::base_<X>{
        typedef detail::base_<X> base_;
    public:
        typedef X                       covariate_type;
        typedef covariate_wrapper<X>    covariate_wrapper_type;
            
        // Construction
        covariate_wrapper();
        covariate_wrapper(const X& x);
        covariate_wrapper(const covariate_wrapper&);
                    
        // Access
        const X& covariate()const;
    };

    // Implementation //
            
    template<typename X>
    covariate_wrapper<X>::covariate_wrapper() : base_(){}
        
    template<typename X>
    covariate_wrapper<X>::covariate_wrapper( const X& x ) : base_(x){}
        
    template<typename X>
    covariate_wrapper<X>::covariate_wrapper(const covariate_wrapper& that)
    :base_(that){}

    template<typename X>
    const X& 
     covariate_wrapper<X>::covariate()const{ return (this->value); }
                
    // Free functions
        
    template<typename X>
    covariate_wrapper<X> 
    make_covariate_wrapper(const X& x){
        typedef covariate_wrapper<X> result_type;
        return result_type(x);
    }

}// model    
}// statistics
}// boost

#endif
