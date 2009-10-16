///////////////////////////////////////////////////////////////////////////////
// statistics::importance_sampling::sampler_deprecated.hpp                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_IMPORTANCE_SAMPLING_SAMPLER_DEPRECATED_HPP_ER_2009
#define BOOST_IMPORTANCE_SAMPLING_SAMPLER_DEPRECATED_HPP_ER_2009
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

// Deprecated because uses categorical_distribution rather than discrete
//
// Samples by SIR given a set of proposal values and their unnormalized weights
//
// R1:  models Range or a reference thereof
// U:   models RandomDistribution (uniform)
//
// Usage:
// typedef S<R1,U> s_;
// typedef s_::result_type res_;
// s_ s(weights,proposal_draws);
// res_ res = s(urng);
//
// NB: pre-sorting the weights from largest to smallest may speed up execution.
template<typename R1, template<typename> class Ur = boost::uniform_real>
class sampler_deprecated{
    typedef typename remove_reference<R1>::type const_values_;
    typedef is_reference<R1>  is_ref_;
    public:
    typedef typename range_value<const_values_>::type result_type;
    private:
    typedef Ur<result_type>                             unif_;
    typedef random::categorical_distribution<unif_>     mult_dist_t;
    public:
    typedef typename mult_dist_t::input_type            input_type;

    sampler_deprecated();
    template<typename R0>
    sampler_deprecated(
        const R0& unnormalized_weights,
        typename call_traits<R1>::param_type values
    );
    sampler_deprecated(const sampler_deprecated& that);
    sampler_deprecated& operator=(const sampler_deprecated& that);

    template<typename U> result_type operator()(U& urng)const;
    const mult_dist_t& categorical_distribution()const;

    // TODO os/is

    private:
    mult_dist_t mult_dist_;
    typename call_traits<R1>::value_type values_;
};

// Definition //

template<typename R1, template<typename> class Ur>
sampler_deprecated<R1,Ur>::sampler_deprecated(){
    BOOST_MPL_ASSERT((
        mpl::not_<is_ref_>
    ));
}

template<typename R1, template<typename> class Ur>
template<typename R0>
sampler_deprecated<R1,Ur>::sampler_deprecated(
    const R0& unnormalized_weights,
    typename call_traits<R1>::param_type values
):mult_dist_(unnormalized_weights),values_(values){
    BOOST_ASSERT(
        size(unnormalized_weights) == size(values_)
    );
}
    
template<typename R1, template<typename> class Ur>
sampler_deprecated<R1,Ur>::sampler_deprecated(
    const sampler_deprecated& that
)
:mult_dist_(that.mult_dist_),values_(that.values_){}

template<typename R1, template<typename> class Ur>
sampler_deprecated<R1,Ur>&
sampler_deprecated<R1,Ur>::operator=(const sampler_deprecated& that){
    if(&that!=this){
        BOOST_MPL_ASSERT((
            mpl::not_<is_ref_>
        ));
        mult_dist_ = that.mult_dist_;
        values_ = that.values_;
    }
    return (*this);
}

// Random        
template<typename R1, template<typename> class Ur>
template<typename U>
typename sampler_deprecated<R1,Ur>::result_type
sampler_deprecated<R1,Ur>::operator()(U& urng)const{
    typedef typename mult_dist_t::result_type k_t;
    k_t k = mult_dist_(urng);
    BOOST_ASSERT( k < size(values_) );
    return (*next(boost::begin(values_),k));
}

// Access
template<typename R1, template<typename> class Ur>
const typename sampler_deprecated<R1,Ur>::mult_dist_t&
sampler_deprecated<R1,Ur>::categorical_distribution()const{ return mult_dist_; }

}// importance_sampling
}// statistics
}// boost

#endif 