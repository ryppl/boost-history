///////////////////////////////////////////////////////////////////////////////
// model::wrap::aggregate::model_dataset.hpp                                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_DATASET_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_MODEL_DATASET_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/model.hpp>
#include <boost/statistics/model/wrap/aggregate/dataset.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename M,typename Rx,typename Ry>
    class model_dataset_ : 
        public  model_<M>, 
        public dataset_<Rx,Ry>
    {
        typedef model_<M> model_;
        typedef dataset_<Rx,Ry> dataset_;
    public:
        typedef model_dataset_<M,Rx,Ry> model_dataset_w_;
            
        // Construction
        model_dataset_();
        model_dataset_(model_ model,dataset_ dataset);
        model_dataset_(
            model_ model,
            typename dataset_::covariates_w_ covs,
            typename dataset_::responses_w_ resps
        );
        model_dataset_(const model_dataset_&);
            
    };

    // Implementation //
            
    template<typename M,typename Rx,typename Ry>
    model_dataset_<M,Rx,Ry>::model_dataset_() : model_(),dataset_(){}
        
    template<typename M,typename Rx,typename Ry>
    model_dataset_<M,Rx,Ry>::model_dataset_(
            model_ model,
            typename dataset_::covariates_w_ covs,
            typename dataset_::responses_w_ resps
    ) : model_( model ), dataset_ (covs,resps) {}
        
    template<typename M,typename Rx,typename Ry>
    model_dataset_<M,Rx,Ry>::model_dataset_(const model_dataset_& that)
    :model_( that ),dataset_( that ) {}

    // Free functions
        
    template<typename M,typename Rx,typename Ry>
    model_dataset_<M,Rx,Ry> 
    make_model_dataset(const M& m,const Rx& rx,const Ry& ry){
        typedef model_dataset_<M,Rx,Ry> result_type;
        return result_type(m,rx,ry);
    }

}// model    
}// statistics
}// boost

#endif
