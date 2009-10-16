///////////////////////////////////////////////////////////////////////////////
// statistics::importance_sampling::sampler.hpp                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_IMPORTANCE_SAMPLING_SAMPLER_HPP_ER_2009
#define BOOST_IMPORTANCE_SAMPLING_SAMPLER_HPP_ER_2009
#include <vector>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/utility.hpp>
#include <boost/range.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
// TODO #include <boost/random/discrete_distributionhpp> when becomes avail
#include <boost/random/discrete_distribution_sw_2009.hpp> 

namespace boost{
namespace statistics{
namespace importance_sampling{

// Samples by SIR given a set of proposal values and their unnormalized weights
//
// R1:  range type of the values 
// W:   type of the weights
//
// Usage:
// typedef S<R1,U> s_;
// typedef s_::result_type res_;
// s_ s(weights,proposal_draws);
// res_ res = s(urng);
template<typename R1,typename W>
class sampler{
    typedef typename remove_reference<R1>::type         const_values_;
    typedef typename remove_cv<const_values_>::type     values_t;    
    typedef typename range_size<values_t>::type         size_;
    typedef is_reference<R1>                            is_ref_;
    public:
    typedef typename range_value<const_values_>::type   result_type;
    private:
    typedef random::discrete_distribution<size_,W>      discr_dist_t;
    public:
    typedef typename discr_dist_t::input_type           input_type;

    sampler();
    template<typename R0>
    sampler(
        const R0& unnormalized_weights,
        typename call_traits<R1>::param_type values
    );
    sampler(const sampler& that);
    sampler& operator=(const sampler& that);

    template<typename U> result_type operator()(U& urng)const;
    const discr_dist_t& discrete_distribution()const;

    // TODO os/is

    private:
    discr_dist_t discr_dist_;
    typename call_traits<R1>::value_type values_;
};

// Definition //

template<typename R1, typename W>
sampler<R1,W>::sampler(){
    BOOST_MPL_ASSERT((
        mpl::not_<is_ref_>
    ));
}

template<typename R1, typename W>
template<typename R0>
sampler<R1,W>::sampler(
    const R0& unnormalized_weights,
    typename call_traits<R1>::param_type values
):discr_dist_(
    boost::begin(unnormalized_weights),
    boost::end(unnormalized_weights)
),values_(values){
    BOOST_ASSERT(
        size(unnormalized_weights) == size(values_)
    );
}
    
template<typename R1, typename W>
sampler<R1,W>::sampler( const sampler& that )
:discr_dist_(that.discr_dist_),values_(that.values_){}

template<typename R1, typename W>
sampler<R1,W>&
sampler<R1,W>::operator=(const sampler& that){
    if(&that!=this){
        BOOST_MPL_ASSERT((
            mpl::not_<is_ref_>
        ));
        discr_dist_ = that.discr_dist_;
        values_ = that.values_;
    }
    return (*this);
}

// Random        
template<typename R1, typename W>
template<typename U>
typename sampler<R1,W>::result_type
sampler<R1,W>::operator()(U& urng)const{
    typedef typename discr_dist_t::result_type k_t;
    k_t k = discr_dist_(urng);
    BOOST_ASSERT( k < size(values_) );
    return (*next(boost::begin(values_),k));
}

// Access
template<typename R1, typename W>
const typename sampler<R1,W>::discr_dist_t&
sampler<R1,W>::discrete_distribution()const{ return discr_dist_; }

}// importance_sampling
}// statistics
}// boost

#endif 