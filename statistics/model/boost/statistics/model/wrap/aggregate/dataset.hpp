///////////////////////////////////////////////////////////////////////////////
// model::wrap::aggregate::dataset.hpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_DATASET_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_WRAP_AGGREGATE_DATASET_HPP_ER_2009
#include <boost/statistics/model/wrap/unary/covariates.hpp>
#include <boost/statistics/model/wrap/unary/responses.hpp>

namespace boost{ 
namespace statistics{
namespace model{
        
    template<typename Rx,typename Ry>
    class dataset_ : 
        public covariates_<Rx>, 
        public responses_<Ry>
    {
        typedef covariates_<Rx> covs_;
        typedef responses_<Ry> resps_;

        public:
        typedef dataset_<Rx,Ry> dataset_w_;
        
        // Construction
        dataset_();
        dataset_(covs_ covs,resps_ resps);
        dataset_(const dataset_&);
            
    };

    // Implementation //
            
    template<typename Rx,typename Ry>
    dataset_<Rx,Ry>::dataset_() : covs_(), resps_(){}
        
    template<typename Rx,typename Ry>
    dataset_<Rx,Ry>::dataset_( covs_ covs, resps_ resps) 
    : covs_( covs ), resps_( resps ){}
        
    template<typename Rx,typename Ry>
    dataset_<Rx,Ry>::dataset_(const dataset_& that)
    :covs_( that ), resps_( that ) {}

    // Free functions
        
    template<typename Rx,typename Ry>
    dataset_<Rx,Ry> 
    make_dataset(const Rx& rx,const Ry& ry){
        typedef dataset_<Rx,Ry> result_type;
        return result_type(rx,ry);
    }

}// model    
}// statistics
}// boost

#endif
