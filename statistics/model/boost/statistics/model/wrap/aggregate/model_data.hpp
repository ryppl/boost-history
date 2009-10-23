///////////////////////////////////////////////////////////////////////////////
// statistics::model::wrap::aggregate::model_data.hpp                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_DATA_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_DATA_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/model.hpp>
#include <boost/statistics/model/wrap/aggregate/data.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename M,typename X,typename Y>
    class model_data_ : 
        public model_wrapper<M>, 
        public data_<X,Y>
    {
        typedef model_wrapper<M> model_;
        typedef data_<X,Y> data_;
    public:
        typedef model_data_<M,X,Y> model_data_type;
            
        // Construction
        model_data_();
        model_data_(model_ model,data_ data);
        model_data_(
            model_ model,
            typename data_::covariate_wrapper_type cov,
            typename data_::response_wrapper_type resp
        );
        model_data_(const model_data_&);
            
    };

    // Implementation //
            
    template<typename M,typename X,typename Y>
    model_data_<M,X,Y>::model_data_() : model_(),data_(){}
        
    template<typename M,typename X,typename Y>
    model_data_<M,X,Y>::model_data_(
            model_ model,
            typename data_::covariate_wrapper_type covs,
            typename data_::response_wrapper_type resps
    ) : model_( model ), data_ (covs,resps) {}
        
    template<typename M,typename X,typename Y>
    model_data_<M,X,Y>::model_data_(const model_data_& that)
    :model_( that ),data_( that ) {}

    // Free functions
        
    template<typename M,typename X,typename Y>
    model_data_<M,X,Y> 
    make_model_data(const M& m,const X& x,const Y& y){
        typedef model_data_<M,X,Y> result_type;
        return result_type(m,x,y);
    }

}// model    
}// statistics
}// boost

#endif
