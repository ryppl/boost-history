///////////////////////////////////////////////////////////////////////////////
// model::functional::log_prior_evaluator.hpp                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MODEL_FUNCTIONAL_LOG_PRIOR_EVALUATOR_HPP_ER_2009
#define BOOST_MODEL_FUNCTIONAL_LOG_PRIOR_EVALUATOR_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/statistics/model/wrap/unary/prior.hpp>

namespace boost{
namespace statistics{
namespace model{  

    // Evaluates the log-likelihood at a parameter values given a model and
    // a dataset.
    //
    // T is the result_type
    template<typename T,typename D>
    class log_prior_evaluator{
        public:
        typedef T result_type;
        typedef prior_<D> prior_type;
        
        // Constructor
        log_prior_evaluator();
        log_prior_evaluator(const prior_type&);
        log_prior_evaluator(const log_prior_evaluator&);
        log_prior_evaluator& operator=(const log_prior_evaluator&);

        // Evaluate
        template<typename P> result_type operator()(const P& p)const;

        // Access
        const prior_type& prior()const;
        
        private:
        prior_type p_;
    };

    // Implementation //
    
    // Construction
    template<typename T,typename D>
    log_prior_evaluator<T,D>::log_prior_evaluator(){}
    
    template<typename T,typename D>
    log_prior_evaluator<T,D>::log_prior_evaluator(
        const prior_type& p
    ):p_(p){}

    template<typename T,typename D>
    log_prior_evaluator<T,D>::log_prior_evaluator(
        const log_prior_evaluator& that
    ):p_(that.p_){}

    template<typename T,typename D>
    log_prior_evaluator<T,D>&
    log_prior_evaluator<T,D>::operator=(
        const log_prior_evaluator& that
    ){
        if(&that!=this){
            p_ = (that.p_);
        }
        return (*this);
    }
    
    // Evaluate
    template<typename T,typename D>
    template<typename P>
    typename log_prior_evaluator<T,D>::result_type 
    log_prior_evaluator<T,D>::operator()(const P& p)const{
        return log_unnormalized_pdf(
            (this->prior()).prior(),
            p
        );
    }

    // Access
    template<typename T,typename D>
    const typename log_prior_evaluator<T,D>::prior_type& 
    log_prior_evaluator<T,D>::prior()const{
        return (this->p_);
    }

}// model
}// statistics
}// boost

#endif