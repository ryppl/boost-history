///////////////////////////////////////////////////////////////////////////////
// model::wrap::aggregate::model_parameter.hpp                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_PARAMETER_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_PARAMETER_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/model.hpp>
#include <boost/statistics/model/wrap/unary/parameter.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename M,typename P>
    class model_parameter_ : 
        public model_<M>, 
        public parameter_<P>
    {
        typedef model_<M> model_;
        typedef parameter_<P> parameter_;

    public:

        typedef model_parameter_<M,P> model_parameter_w_;
            
        // Construction
        model_parameter_();
        model_parameter_(model_ model,parameter_ par);
        model_parameter_(const model_parameter_&);
            
    };

    // Implementation //
            
    template<typename M,typename P>
    model_parameter_<M,P>::model_parameter_() : model_(),parameter_(){}
        
    template<typename M,typename P>
    model_parameter_<M,P>::model_parameter_( model_ m, parameter_ p )
     : model_( m ), parameter_( p ){}
        
    template<typename M,typename P>
    model_parameter_<M,P>::model_parameter_(const model_parameter_& that)
    :model_( that ),parameter_( that ) {}

    // Free functions
        
    template<typename M,typename P>
    model_parameter_<M,P> 
    make_model_parameter(const M& m,const P& p){
        typedef model_parameter_<M,P> result_type;
        return result_type(m,p);
    }

}// model    
}// statistics
}// boost

#endif
