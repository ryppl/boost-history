///////////////////////////////////////////////////////////////////////////////
// model::functional::log_likelihood_evaluator.hpp                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_LIKELIHOOD_EVALUATOR_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_LIKELIHOOD_EVALUATOR_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/statistics/model/wrap/aggregate/model_parameter.hpp>
#include <boost/statistics/model/wrap/aggregate/model_dataset.hpp>
#include <boost/statistics/model/algorithm/log_likelihood.hpp>

namespace boost{ 
namespace statistics{
namespace model{  

    // Evaluates the log-likelihood at a parameter values given a model and
    // a dataset.
    //
    // T is the result_type
    template<typename T,typename M,typename Rx,typename Ry>
    class log_likelihood_evaluator{
        public:
        typedef T result_type;
        typedef model_dataset_<M,Rx,Ry> model_dataset_type;
        
        // Constructor
        log_likelihood_evaluator();
        log_likelihood_evaluator(const model_dataset_type&);
        log_likelihood_evaluator(const log_likelihood_evaluator&);
        log_likelihood_evaluator& operator=(const log_likelihood_evaluator&);

        // Evaluate
        template<typename P> result_type operator()(const P& p)const;

        // Access
        const model_dataset_type& model_dataset()const;
        
        private:
        model_dataset_type md_;
    };

    // Implementation //
    
    // Construction
    template<typename T,typename M,typename Rx,typename Ry>
    log_likelihood_evaluator<T,M,Rx,Ry>::log_likelihood_evaluator(){}
    
    template<typename T,typename M,typename Rx,typename Ry>
    log_likelihood_evaluator<T,M,Rx,Ry>::log_likelihood_evaluator(
        const model_dataset_type& md
    ):md_(md){}

    template<typename T,typename M,typename Rx,typename Ry>
    log_likelihood_evaluator<T,M,Rx,Ry>::log_likelihood_evaluator(
        const log_likelihood_evaluator& that
    ):md_(that.md_){}

    template<typename T,typename M,typename Rx,typename Ry>
    log_likelihood_evaluator<T,M,Rx,Ry>&
    log_likelihood_evaluator<T,M,Rx,Ry>::operator=(
        const log_likelihood_evaluator& that
    ){
        if(&that!=this){
            md_ = (that.md_);
        }
        return (*this);
    }
    
    // Evaluate
    template<typename T,typename M,typename Rx,typename Ry>
    template<typename P>
    typename log_likelihood_evaluator<T,M,Rx,Ry>::result_type 
    log_likelihood_evaluator<T,M,Rx,Ry>::operator()(const P& p)const{
        return log_likelihood<T>(
            make_model_parameter(
                model_dataset().model(),
                p
            ),
            boost::begin( model_dataset().covariates() ),
            boost::end( model_dataset().covariates() ),
            boost::begin( model_dataset().responses() )
        );
    }

    // Access
    template<typename T,typename M,typename Rx,typename Ry>
    const typename log_likelihood_evaluator<T,M,Rx,Ry>::model_dataset_type& 
    log_likelihood_evaluator<T,M,Rx,Ry>::model_dataset()const{
        return (this->md_);
    }

}// model
}// statistics
}// boost

#endif