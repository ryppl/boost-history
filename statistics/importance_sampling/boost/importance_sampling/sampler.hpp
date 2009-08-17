///////////////////////////////////////////////////////////////////////////////
// is::sampler.hpp                                                           //
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
#include <boost/random/categorical_distribution.hpp>

namespace boost{
namespace is{

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
class sampler{
    typedef typename remove_reference<R1>::type const_values_;
    typedef is_reference<R1>  is_ref_;
    public:
    typedef typename range_value<const_values_>::type result_type;
    private:
    typedef Ur<result_type>                             unif_;
    typedef random::categorical_distribution<unif_>     mult_dist_t;
    public:
    typedef typename mult_dist_t::input_type            input_type;

    sampler();
    template<typename R0>
    sampler(
        const R0& unnormalized_weights,
        typename call_traits<R1>::param_type values
    );
    sampler(const sampler& that);
    sampler& operator=(const sampler& that);

    template<typename U> result_type operator()(U& urng)const;
    const mult_dist_t& categorical_distribution()const;

    // TODO os/is

    private:
    mult_dist_t mult_dist_;
    typename call_traits<R1>::value_type values_;
};

// Definition //

template<typename R1, template<typename> class Ur>
sampler<R1,Ur>::sampler(){
    BOOST_MPL_ASSERT((
        mpl::not_<is_ref_>
    ));
}

template<typename R1, template<typename> class Ur>
template<typename R0>
sampler<R1,Ur>::sampler(
    const R0& unnormalized_weights,
    typename call_traits<R1>::param_type values
):mult_dist_(unnormalized_weights),values_(values){
    BOOST_ASSERT(
        size(unnormalized_weights) == size(values_)
    );
}
    
template<typename R1, template<typename> class Ur>
sampler<R1,Ur>::sampler(
    const sampler& that
)
:mult_dist_(that.mult_dist_),values_(that.values_){}

template<typename R1, template<typename> class Ur>
sampler<R1,Ur>&
sampler<R1,Ur>::operator=(const sampler& that){
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
typename sampler<R1,Ur>::result_type
sampler<R1,Ur>::operator()(U& urng)const{
    typedef typename mult_dist_t::result_type k_t;
    k_t k = mult_dist_(urng);
    BOOST_ASSERT( k < size(values_) );
    return (*next(boost::begin(values_),k));
}

// Access
template<typename R1, template<typename> class Ur>
const typename sampler<R1,Ur>::mult_dist_t&
sampler<R1,Ur>::categorical_distribution()const{ return mult_dist_; }

}//random
}//boost

#endif 