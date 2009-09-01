///////////////////////////////////////////////////////////////////////////////
// model::wrap::aggregate::model_covariate_parameter.hpp                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_COVARIATE_PARAMETER_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_COVARIATE_PARAMETER_HPP_ER_2009
#include <boost/statistics/model/wrap/aggregate/model_parameter.hpp>
#include <boost/statistics/model/wrap/unary/covariate.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename M,typename X,typename P>
    class model_covariate_parameter_ : 
        public model_parameter_<M,P>, 
        public covariate_<X>
    {
        typedef model_parameter_<M,P>       model_parameter_;
        typedef covariate_<X>               covariate_;

        public:

        typedef model_covariate_parameter_<M,X,P> model_covariate_parameter_w_;
            
        // Construction
        model_covariate_parameter_();
        model_covariate_parameter_(model_parameter_ mp,covariate_ cov);
        model_covariate_parameter_(
            typename model_parameter_::model_w_ model,
            covariate_ cov, 
            typename model_parameter_::parameter_w_ par
        );
        model_covariate_parameter_(const model_covariate_parameter_&);
            
    };

    // Implementation //
            
    template<typename M,typename X,typename P>
    model_covariate_parameter_<M,X,P>::model_covariate_parameter_() 
    : model_parameter_(),covariate_(){}
        
    template<typename M,typename X,typename P>
    model_covariate_parameter_<M,X,P>::model_covariate_parameter_( 
        model_parameter_ mp, covariate_ cov 
    )
     : model_parameter_( mp ), covariate_( cov ){}

    template<typename M,typename X,typename P>
    model_covariate_parameter_<M,X,P>::model_covariate_parameter_( 
        typename model_parameter_::model_w_ m, 
        covariate_ cov, 
        typename model_parameter_::parameter_w_ p
    )
     : model_parameter_( m, p ), covariate_( cov ){}
        
    template<typename M,typename X,typename P>
    model_covariate_parameter_<M,X,P>::model_covariate_parameter_(
        const model_covariate_parameter_& that
    ) : model_parameter_( that ), covariate_( that ) {}

    // Free functions
        
    template<typename M,typename X,typename P>
    model_covariate_parameter_<M,X,P> 
    make_model_covariate_parameter(const M& m,const X& x,const P& p){
        typedef model_covariate_parameter_<M,X,P> result_type;
        return result_type(m,x,p);
    }

}// model    
}// statistics
}// boost

#endif
