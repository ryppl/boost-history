///////////////////////////////////////////////////////////////////////////////
// model::wrap::aggregate::prior_model_dataset.hpp                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_PRIOR_MODEL_DATASET_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_PRIOR_MODEL_DATASET_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/prior.hpp>
#include <boost/statistics/model/wrap/aggregate/model_dataset.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename D,typename M,typename Rx,typename Ry>
    class prior_model_dataset_ : 
        public prior_<D>,
        public model_dataset_<M,Rx,Ry>
    {
        typedef prior_<D> prior_;
        typedef model_dataset_<M,Rx,Ry> model_dataset_;
    public:
        typedef prior_model_dataset_<D,M,Rx,Ry> prior_model_dataset_w_;
            
        // Construction
        prior_model_dataset_();
        prior_model_dataset_(prior_ prior,model_dataset_ model_dataset);
        prior_model_dataset_(
            prior_ prior,
            typename model_dataset_::model_w_ m,
            typename model_dataset_::covariates_w_ covs,
            typename model_dataset_::responses_w_ resps
        );
        prior_model_dataset_(const prior_model_dataset_&);
            
    };

    // Implementation //
            
    template<typename D,typename M,typename Rx,typename Ry>
    prior_model_dataset_<D,M,Rx,Ry>::prior_model_dataset_() 
        : prior_(),model_dataset_(){}
        
    template<typename D,typename M,typename Rx,typename Ry>
    prior_model_dataset_<D,M,Rx,Ry>::prior_model_dataset_(
            prior_ prior,
            typename model_dataset_::model_w_ m,
            typename model_dataset_::covariates_w_ covs,
            typename model_dataset_::responses_w_ resps
    ) : prior_( prior ), model_dataset_ ( m, covs, resps) {}
        
    template<typename D,typename M,typename Rx,typename Ry>
    prior_model_dataset_<D,M,Rx,Ry>::prior_model_dataset_(
        const prior_model_dataset_& that)
    :prior_( that ),model_dataset_( that ) {}

    // Free functions
        
    template<typename D,typename M,typename Rx,typename Ry>
    prior_model_dataset_<D,M,Rx,Ry> 
    make_prior_model_dataset(const D& d,const M& m,const Rx& rx,const Ry& ry){
        typedef prior_model_dataset_<D,M,Rx,Ry> result_type;
        return result_type(d,m,rx,ry);
    }

}// model    
}// statistics
}// boost

#endif

