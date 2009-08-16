///////////////////////////////////////////////////////////////////////////////
// dist_random::random::sample.hpp                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_RANDOM_SAMPLE_N_HPP_ER_2009
#define BOOST_DIST_RANDOM_RANDOM_SAMPLE_N_HPP_ER_2009
#include <boost/random/variate_generator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/dist_random/meta/dist_random_result.hpp>
#include <boost/dist_random/functional/make_random.hpp>

namespace boost{

    // can't call it random because that name is also namespace

    // Generates one draw from a distribution
    // If you need to call this repeatedly, generate_n is faster
    template<typename D,typename U> 
    typename lazy_enable_if<
        math::is_scalar_dist<D>,
        dist_random_result<D> 
    >::type
    sample(const D& dist, U& urng){
        typedef dist_random<D> dr_;
        typedef typename dr_::type r_; 
        typedef variate_generator<U&,r_> vg_;
        // Costly but necessary to guarantee U is mapped to agree with 
        // dist_random_input<D>::type
        vg_ vg(urng,make_random(dist));
        return vg();
    }
        
}

#endif
