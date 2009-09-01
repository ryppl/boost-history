///////////////////////////////////////////////////////////////////////////////
// model::wrap::unary::covariates.hpp                                        //
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
    class covariates_ : detail::base_<Rx>{
        typedef detail::base_<Rx> base_;
    public:
        typedef Rx                  covariates_type;
        typedef covariates_<Rx>     covariates_w_;

        // Construction
        covariates_();
        covariates_(const Rx& x);
        covariates_(const covariates_&);
            
        // Access
        const Rx& covariates()const;
    };

    // Implementation //
            
    template<typename Rx>
    covariates_<Rx>::covariates_() : base_(){}
        
    template<typename Rx>
    covariates_<Rx>::covariates_( const Rx& rx ) : base_(rx){}
        
    template<typename Rx>
    covariates_<Rx>::covariates_(const covariates_& that)
    :base_(that){}

    template<typename Rx>
    const Rx& covariates_<Rx>::covariates()const{ return (this->value); }
                
    // Free functions
        
    template<typename Rx>
    covariates_<Rx> 
    make_covariates(const Rx& rx){
        typedef covariates_<Rx> result_type;
        return result_type(rx);
    }

}// model    
}// statistics
}// boost

#endif
