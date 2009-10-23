///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::parameter.hpp                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_PARAMETER_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_PARAMETER_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename X>
    class parameter_wrapper : detail::base_<X>{
        typedef detail::base_<X> base_;
    public:
        typedef X                       parameter_type;
        typedef parameter_wrapper<X>    parameter_wrapper_type;
            
        // Construction
        parameter_wrapper();
        parameter_wrapper(const X& x);
        parameter_wrapper(const parameter_wrapper&);
            
        // Access
        const X& parameter()const;
    };

    // Implementation //
            
    template<typename X>
    parameter_wrapper<X>::parameter_wrapper() : base_(){}
        
    template<typename X>
    parameter_wrapper<X>::parameter_wrapper( const X& x ) : base_(x){}
        
    template<typename X>
    parameter_wrapper<X>::parameter_wrapper(const parameter_wrapper& that)
    :base_(that){}

    template<typename X>
    const X& parameter_wrapper<X>::parameter()const{ return (this->value); }
                
    // Free functions
        
    template<typename X>
    parameter_wrapper<X> 
    make_parameter_wrapper(const X& x){
        typedef parameter_wrapper<X> result_type;
        return result_type(x);
    }

}// model    
}// statistics
}// boost

#endif
