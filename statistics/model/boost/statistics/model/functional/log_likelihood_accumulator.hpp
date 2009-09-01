///////////////////////////////////////////////////////////////////////////////
// model::functional::log_likelihood_accumulator.hpp                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_LIKELIHOOD_ACCUMULATOR_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_FUNCTIONAL_LOG_LIKELIHOOD_ACCUMULATOR_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/statistics/model/wrap/aggregate/model_parameter.hpp>
#include <boost/statistics/model/wrap/aggregate/model_data.hpp>
#include <boost/joint_dist/unscope/log_unnormalized_pdf.hpp>
#include <boost/scalar_dist/unscope/log_unnormalized_pdf.hpp>

namespace boost{

    // fwd declare
    template<typename T,typename M,typename X,typename Y,typename P>
    T log_likelihood(
        statistics::model::model_data_<M,X,Y>,
        const P&
    );

namespace statistics{
namespace model{

// Functor that accumulates the data contribution to the likelihood of a model 
// and its parameter.
//
// Used by algorithm::log_likelihood        
template<
    typename T, // result_type
    typename M, // Model
    typename P  // parameter
>
class log_likelihood_accumulator : boost::addable<
    log_likelihood_accumulator<T,M,P>
>{
public:
    typedef T                        result_type;
    typedef model_parameter_<M,P>    model_parameter_type;


    // Construction
    log_likelihood_accumulator();
    log_likelihood_accumulator(model_parameter_type);
    log_likelihood_accumulator(const log_likelihood_accumulator& );
    log_likelihood_accumulator& operator=(const log_likelihood_accumulator& );

    // Operator
    log_likelihood_accumulator& operator+=(
        const log_likelihood_accumulator& that
    );
    
    // Update
    template<typename X,typename Y> 
    result_type operator()(const X&,const Y& y);
    
    // Access
    const model_parameter_type& model_parameter()const;
    result_type value()const;
    
private:
    model_parameter_type mp_;
    result_type cum_sum_;
    static result_type zero_;
};
    
    // Implementation //

template<typename T,typename M,typename P>
typename log_likelihood_accumulator<T,M,P>::result_type
log_likelihood_accumulator<T,M,P>::zero_ = static_cast<result_type>(0);    

// Construction
template<typename T,typename M,typename P>
log_likelihood_accumulator<T,M,P>::log_likelihood_accumulator()
:mp_(),cum_sum_(zero_){}

template<typename T,typename M,typename P>
log_likelihood_accumulator<T,M,P>::log_likelihood_accumulator(
    model_parameter_type mp
):mp_(mp),cum_sum_(zero_){}

template<typename T,typename M,typename P>
log_likelihood_accumulator<T,M,P>::log_likelihood_accumulator(
    const log_likelihood_accumulator& that
):mp_(that.mp_),cum_sum_(that.cum_sum_){}

template<typename T,typename M,typename P>
log_likelihood_accumulator<T,M,P>& 
log_likelihood_accumulator<T,M,P>::operator=(
    const log_likelihood_accumulator& that
){
    if(&that!=this){
        mp_ = that.mp_;
        cum_sum_ = that.cum_sum_;
    }
    return (*this);
}

// Operator

template<typename T,typename M,typename P>
log_likelihood_accumulator<T,M,P>& 
log_likelihood_accumulator<T,M,P>::operator+=(
    const log_likelihood_accumulator& that
){
    (this->cum_sum_)+= that.value();
}
    
// Update
template<typename T,typename M,typename P>
template<typename X,typename Y>
typename log_likelihood_accumulator<T,M,P>::result_type 
log_likelihood_accumulator<T,M,P>::operator()(const X& x,const Y& y){

    result_type l = log_likelihood<T>(
        make_model_data(
            model_parameter().model(),
            x,
            y
        ),
        model_parameter().parameter()
    );
    cum_sum_ += l;
    return cum_sum_;
}

// Access

template<typename T,typename M,typename P>
const typename log_likelihood_accumulator<T,M,P>::model_parameter_type&
log_likelihood_accumulator<T,M,P>::model_parameter()const{ return mp_; }

template<typename T,typename M,typename P>
typename log_likelihood_accumulator<T,M,P>::result_type 
log_likelihood_accumulator<T,M,P>::value()const{ return cum_sum_; }

}// model
}// statistics
}// boost

#endif