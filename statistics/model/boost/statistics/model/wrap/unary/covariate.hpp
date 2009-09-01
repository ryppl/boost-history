///////////////////////////////////////////////////////////////////////////////
// model::wrap::unary::covariate.hpp                                         //
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
    class covariate_ : detail::base_<X>{
        typedef detail::base_<X> base_;
    public:
        typedef X                       covariate_type;
        typedef covariate_<X>           covariate_w_;
            
        // Construction
        covariate_();
        covariate_(const X& x);
        covariate_(const covariate_&);
            
        // Access
        const X& covariate()const;
    };

    // Implementation //
            
    template<typename X>
    covariate_<X>::covariate_() : base_(){}
        
    template<typename X>
    covariate_<X>::covariate_( const X& x ) : base_(x){}
        
    template<typename X>
    covariate_<X>::covariate_(const covariate_& that)
    :base_(that){}

    template<typename X>
    const X& covariate_<X>::covariate()const{ return (this->value); }
                
    // Free functions
        
    template<typename X>
    covariate_<X> 
    make_covariate_(const X& x){
        typedef covariate_<X> result_type;
        return result_type(x);
    }

}// model    
}// statistics
}// boost

#endif
