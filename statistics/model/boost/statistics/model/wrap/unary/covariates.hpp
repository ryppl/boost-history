///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::unary::covariates.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_UNARY_COVARIATES_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_UNARY_COVARIATES_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/detail/base.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename Rx>
    class covariates_wrapper : detail::base_<Rx>{
        typedef detail::base_<Rx> base_;
    public:
        typedef Rx                          covariates_type;
        typedef covariates_wrapper<Rx>      covariates_wrapper_type;

        // Construction
        covariates_wrapper();
        covariates_wrapper(const Rx& x);
        covariates_wrapper(const covariates_wrapper&);
            
        // Access
        const Rx& covariates()const;
    };

    // Implementation //
            
    template<typename Rx>
    covariates_wrapper<Rx>::covariates_wrapper() : base_(){}
        
    template<typename Rx>
    covariates_wrapper<Rx>::covariates_wrapper( const Rx& rx ) : base_(rx){}
        
    template<typename Rx>
    covariates_wrapper<Rx>::covariates_wrapper(const covariates_wrapper& that)
    :base_(that){}

    template<typename Rx>
    const Rx& covariates_wrapper<Rx>::covariates()const{ 
        return (this->value); 
    }
                
    // Free functions
        
    template<typename Rx>
    covariates_wrapper<Rx> 
    make_covariates_wrapper(const Rx& rx){
        typedef covariates_wrapper<Rx> result_type;
        return result_type(rx);
    }

}// model    
}// statistics
}// boost

#endif
