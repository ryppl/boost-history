///////////////////////////////////////////////////////////////////////////////
// statistics::model::functional::log_posterior_evaluator.hpp                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_POSTERIOR_EVALUATOR_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_POSTERIOR_EVALUATOR_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/binary_op/algorithm/for_each.hpp>
#include <boost/joint_dist/unscope/log_unnormalized_pdf.hpp>
#include <boost/scalar_dist/unscope/log_unnormalized_pdf.hpp>
#include <boost/statistics/model/wrap/aggregate/prior_model_dataset.hpp>
#include <boost/statistics/model/functional/log_likelihood_evaluator.hpp>

namespace boost{ 
namespace statistics{
namespace model{  

    // Augments algorithm::log_likehood with a prior of type D
    //
    // Requirements:
    // Let d denote an instance of D and p a parameter, then
    // log_unnormalized_pdf(p_dist,p) must return an object of type T    
    template<typename T,typename D,typename M,typename Rx,typename Ry>
    class log_posterior_evaluator : log_likelihood_evaluator<T,M,Rx,Ry>{
        typedef log_likelihood_evaluator<T,M,Rx,Ry> super_;
        public:
        typedef prior_model_dataset_<D,M,Rx,Ry> prior_model_dataset_type;
        typedef typename prior_model_dataset_type::prior_type prior_type;
        typedef typename super_::result_type result_type;
        
        // Constructor
        log_posterior_evaluator();
        log_posterior_evaluator(const prior_model_dataset_type&);
        log_posterior_evaluator(const log_posterior_evaluator&);
        log_posterior_evaluator& operator=(const log_posterior_evaluator&);

        // Evaluate
        template<typename P> 
        result_type operator()(const P& p)const;

        // Access
        const prior_model_dataset_type& prior_model_dataset()const;
        
        private:
        prior_model_dataset_type pmd_;
    };

    // Implementation //
    
    // Construction
    template<typename T,typename D,typename M,typename Rx,typename Ry>
    log_posterior_evaluator<T,D,M,Rx,Ry>::log_posterior_evaluator():
    super_(){}
    
    template<typename T,typename D,typename M,typename Rx,typename Ry>
    log_posterior_evaluator<T,D,M,Rx,Ry>::log_posterior_evaluator(
        const prior_model_dataset_type& pmd
    ):super_(pmd),pmd_(pmd){}

    template<typename T,typename D,typename M,typename Rx,typename Ry>
    log_posterior_evaluator<T,D,M,Rx,Ry>::log_posterior_evaluator(
        const log_posterior_evaluator& that
    ):super_(that),pmd_(that.pmd_){}

    template<typename T,typename D,typename M,typename Rx,typename Ry>
    log_posterior_evaluator<T,D,M,Rx,Ry>&
    log_posterior_evaluator<T,D,M,Rx,Ry>::operator=(
        const log_posterior_evaluator& that
    ){
        if(&that!=this){
            super_::operator=(that);
            pmd_ = (that.pmd_);
        }
        return (*this);
    }
    
    // Evaluate
    template<typename T,typename D,typename M,typename Rx,typename Ry>
    template<typename P>
    typename log_posterior_evaluator<T,D,M,Rx,Ry>::result_type 
    log_posterior_evaluator<T,D,M,Rx,Ry>::operator()(const P& p)const{
        const super_& super = static_cast<const super_&>(*this);
        result_type res =  super(p);
        res += log_unnormalized_pdf(
            prior_model_dataset().prior(),
            p
        );
        return res;
    }

    // Access
    template<typename T,typename D,typename M,typename Rx,typename Ry>
    const 
        typename 
            log_posterior_evaluator<T,D,M,Rx,Ry>::prior_model_dataset_type& 
    log_posterior_evaluator<T,D,M,Rx,Ry>::prior_model_dataset()const{
        return (this->pmd_);
    }

}// model
}// statistics
}// boost

#endif